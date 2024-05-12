#include <Headers.hpp>

#define MINIAUDIO_IMPLEMENTATION // Attention à ne pas oublier cette ligne, sinon miniaudio n'est pas correctement inclus
#include "miniaudio.h"

// Temporaire (tant que ces variables n'ont pas été mise dans des classes)
#include "variables.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

GLuint programID, programID_HUD,programID_Entity;

bool isImGuiShow = true;
bool switchToEditor = false;

// Caméra
glm::vec3 camera_position  = glm::vec3(0.0f, 5.0f, 5.0f);
glm::vec3 camera_target = glm::vec3(1.0f,0.0f,-1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f,  0.0f);

bool cameraOrbitale = false;
bool cameraLibre = false; // Caméra libre par défaut
bool cameraMouseLibre = false;
bool cameraMousePlayer = true;
int speedCam = 50;
double previousX = SCREEN_WIDTH / 2;
double previousY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
float phi = -90.0f;
float theta = 0.0f;
float FoV = 75.0f;
float FoV_running = 80.0f;

TerrainControler *terrainControler;
Player *player;
Hitbox *hitboxPlayer;
bool hasUpdate; // A rentrer dans la classe Hitbox plus tard
bool isRunning = false;
bool isHoldingShift = false;

// --------------------------------
// Temporaire
float angleZombie=0.0f;
float angleCochon=0.0f;
bool walkZombie=false;
bool walkCochon=false;
bool fightZombie=false;
bool dieZombie=false;
float accumulateurAnimation = 0.0f; // Va servir à faire animations indépendantes du nombre de frame
// --------------------------------

int blockInHotbar[9] = {23,29,1,11,12,13,20,26,33}; // Blocs qui sont dans la hotbar
int indexHandBlock = 0;
int handBlock = blockInHotbar[indexHandBlock]; // ID du block que le joueur est en train de poser (se modifie à la molette de la souris)

bool showHud = true;
int isShadow = 1; // 1 si on utilise les ombres dans le shader, 0 sinon
bool modeJeu = false; // true pour créatif, false pour survie

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ 
    glViewport(0,0,width,height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_F && action == GLFW_PRESS){
        isImGuiShow = !isImGuiShow;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS){ // Plus tard on fera une checkbox ImGui pour changer la lumière
        glUseProgram(programID);
        isShadow = 1 - isShadow; // Inverse la valeur
        glUniform1i(glGetUniformLocation(programID, "isShadow"), isShadow);
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
        player->applyAcceleration(true);
        isRunning = true;
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE){
        if (!isHoldingShift){
            player->applyAcceleration(false);
        }
        isHoldingShift = false;
        isRunning = false;
    }
    // Pour sortir de la caméra à la souris (plus tard ce sera la touche qui ouvre l'inventaire, et donc affiche la souris dans la fenêtre)
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
        if (!switchToEditor){ 
            if (cameraMousePlayer){
                cameraMouseLibre = false;
                cameraMousePlayer = false;
                cameraOrbitale = false;
                cameraLibre = true;
            }else{
                cameraMouseLibre = false;
                cameraMousePlayer = true;
                cameraOrbitale = false;
                cameraLibre = false;
            }
        }else{
            if (cameraMouseLibre){
                cameraMouseLibre = false;
                cameraMousePlayer = false;
                cameraOrbitale = false;
                cameraLibre = true;
            }else{
                cameraMouseLibre = true;
                cameraMousePlayer = false;
                cameraOrbitale = false;
                cameraLibre = false;
            }
        }
    }

    // ---------------------------------------------------------
    // Joue les animations du zombie et du cochon (temporaire, on utilisera une classe Agent)
    // Marche
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){ 
            walkZombie = !walkZombie;
    }
    // Attaque
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){ 
            fightZombie = !fightZombie;
    }

    // Meurt
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){ 
            dieZombie = !dieZombie;
    }

    // Animation du cochon
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){ 
            walkCochon = !walkCochon;
    }
    // ---------------------------------------------------------
}

void processInput(GLFWwindow* window){ 
    float camera_speed = (float)speedCam * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
    
    // Déplacement du joueur
    if(cameraMousePlayer){
        // On est obligé de vérifier que le joueur n'appuie pas sur 2 touches opposées en même temps (sinon motion est nulle et ça fait des valeurs NaN)
        // C'est à ça que servent x_axis et z_axis
        bool x_axis = false;
        bool z_axis = false;
        glm::vec3 bottomPlayer = hitboxPlayer->getBottomPoint();
        glm::vec3 motion = glm::vec3(0.0f,0.0f,0.0f); // On va accumuler les déplacements pour que le joueur puisse se déplacer en diagonale
        glm::vec3 cross_point;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ // Déplacement en avant
            if (hitboxPlayer->getLateralMovePossible(false,1.0, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::vec3(camera_target[0],0.0f,camera_target[2]);
                z_axis = true;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ // Déplacement vers la gauche
            if (hitboxPlayer->getLateralMovePossible(true,-1.0, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::normalize(cross_point);
                x_axis = true;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ // Déplacement en arrière
            if (hitboxPlayer->getLateralMovePossible(false,-1.0, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::vec3(camera_target[0],0.0f,camera_target[2])*-1.0f;
                z_axis = !z_axis;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ // Déplacement vers la droite
            if (hitboxPlayer->getLateralMovePossible(true,1.0, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::normalize(cross_point);
                x_axis = !x_axis;
            }
        }

        if (x_axis || z_axis){
            hitboxPlayer->move(glm::normalize(motion)*player->getPlayerSpeed()*deltaTime); // Attention à bien normaliser le vecteur de déplacement final (ça règle le problème de sqrt(2))
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            if (hitboxPlayer->getCanJump()){
                hitboxPlayer->resetJumpForce();
                hitboxPlayer->setCanJump(false);
            }
        }

    }else{
        // Avancer/Reculer la caméra
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            camera_position += (camera_speed / 5.f) * camera_target;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            camera_position -= (camera_speed / 5.f) * camera_target;
        }
        // Monter/Descendre la caméra
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            camera_position += (camera_speed / 5.f) * camera_up;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
            camera_position -= (camera_speed / 5.f) * camera_up;
        }
        // Déplacer vers la droite/gauche la caméra
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera_position += (camera_speed / 5.f) * glm::normalize(glm::cross(camera_target,camera_up));;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera_position -= (camera_speed / 5.f) * glm::normalize(glm::cross(camera_target,camera_up));;
        }
    }
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos){
    if (cameraMouseLibre || cameraMousePlayer){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Pour masquer la souris sur la fenêtre

        if (firstMouse){
            previousX = xpos;
            previousY = ypos;
            firstMouse = false;
        }

        double deltaX = xpos - previousX;
        double deltaY = previousY - ypos;

        phi += deltaX*0.05f;
        theta += deltaY*0.05f;
        // On empêche la caméra de s'inverser
        theta = std::max(std::min(theta, 89.99f), -89.99f);

        float x = cos(glm::radians(phi)) * cos(glm::radians(theta));
        float y = sin(glm::radians(theta));
        float z = sin(glm::radians(phi)) * cos(glm::radians(theta));
        camera_target = glm::normalize(glm::vec3(x,y,z));

        previousX=xpos;
        previousY=ypos;

    }else{
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Afficher le curseur
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        terrainControler->setMouseLeftClickHold(true);
        terrainControler->setPreviousIdInChunk(-2);
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        terrainControler->setMouseLeftClickHold(false);
        terrainControler->setAccumulation(0.0f);
        terrainControler->setPreviousIdInChunk(-2);
        glUseProgram(programID);
        glUniform1i(glGetUniformLocation(programID, "indexBlockToBreak"), terrainControler->getPreviousIdInChunk());
    }else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        terrainControler->tryCreateBlock(camera_target, camera_position, handBlock);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    indexHandBlock = (indexHandBlock + (yoffset > 0 ? -1 : 1) + 9) % 9; // 9 emplacements dans la hotbar
    glUniform1i(glGetUniformLocation(programID_HUD, "selectLocation"), indexHandBlock);
    handBlock = blockInHotbar[indexHandBlock];
}

/*
// Callback qui permet de jouer en boucle une musique (pour l'instant ne fonctionne pas)
// Ce n'est peut être pas une si bonne idée d'utiliser un callback (à voir)
void restart_callback(ma_sound* pSound, void* pUserData) {
    ma_sound_seek_to_pcm_frame(pSound, 0);
    ma_sound_start(pSound);
}
*/

int main(){
    if( !glfwInit()){
        fprintf( stderr, "Failed to initialize GLFW\n" );
        //getchar();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    Window *window_object = new Window(3, 3, SCREEN_WIDTH, SCREEN_HEIGHT, "Projet Moteur de jeux");
    window_object->setup_GLFW();
    GLFWwindow* window = window_object->get_window();
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // On définit le callback à appeler lors du redimensionnement de la fenêtre
    glClearColor(0.36f, 0.61f, 1.0f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE); // Attention à la construction des triangles
    // Pour utiliser de la transparence dans le fragment shader (par exemple pour le bloc de glace)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ---------------------------------------------------------
    // Temporaire : Gestion du son (On fera une classe à part)
    ma_result result;
    ma_engine engine;
    ma_sound sound;
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS){
        std::cout << "Failed to initialized miniaudio engine\n";
        ma_engine_uninit(&engine);
        return -1;
    }
    result = ma_sound_init_from_file(&engine, "../Sound/BackgroundMusic/black3.mp3", 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS){
        std::cout << "Impossible de charger le son\n";
        ma_engine_uninit(&engine);
        return -1;
    }
    // Utilisation du callback qui relance la musique une fois terminé
    // Le callback est bien appelé mais la musique ne veut pas se relancer
    //ma_sound_set_end_callback(&sound, restart_callback, NULL);
    ma_sound_set_looping(&sound,true); // Joue la musique en boucle
    ma_sound_start(&sound);

    // Pour jouer une musique en donnant directement le nom du fichier :
    //ma_engine_play_sound(&engine,"../Sound/BackgroundMusic/black1.mp3",NULL);
    // Quelques fonctions qui pourrait être utile :
    // ma_sound_seek_to_pcm_frame(&sound, frameIndex);
    // ma_sound_get_data_format(&sound, &format, &channels, &sampleRate, pChannelMap, channelMapCapacity);
    // ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
    // ma_sound_get_length_in_pcm_frames(&sound, &length);

    // ---------------------------------------------------------

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    programID = LoadShaders("../shaders/vertexShader.vert", "../shaders/fragmentShader.frag");
    programID_HUD = LoadShaders("../shaders/hud_vertex.vert", "../shaders/hud_frag.frag");
    programID_Entity = LoadShaders("../shaders/entity_vertex.vert", "../shaders/entity_frag.frag");

    std::vector<std::string> nomStructure;
    nomStructure.push_back("../Structures/Tree.txt");
    nomStructure.push_back("../Structures/Tree_2.txt");
    nomStructure.push_back("../Structures/House_1.txt");
    terrainControler = new TerrainControler(3, 3, 1, 3, 1000, 4, nomStructure);
    player = new Player(glm::vec3(-0.5f,10.0f,-0.5f), 1.8f, 0.6f, 6.0f, 1.5f); // Le joueur fait 1.8 bloc de haut, et 0.6 bloc de large et de long
    hitboxPlayer = player->getHitbox();

    Skybox *skybox = new Skybox();

    glUseProgram(programID_HUD);
    Hud *hud = new Hud(SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform1i(glGetUniformLocation(programID_HUD, "selectLocation"), indexHandBlock);
    glUniform1iv(glGetUniformLocation(programID_HUD, "blockHotbar"), 9, blockInHotbar);
    hud->loadHud();

    glUseProgram(programID);
    // On envoie ces valeurs par défaut aux shaders
    glUniform1i(glGetUniformLocation(programID, "isShadow"), isShadow);
    glUniform1i(glGetUniformLocation(programID, "indexBlockToBreak"), terrainControler->getPreviousIdInChunk());
    
    GLuint ModelMatrix = glGetUniformLocation(programID,"Model");
    // La matrice Model ne sera pas envoyé aux shaders des entités (ça ne sert à rien)
    GLuint ViewMatrix = glGetUniformLocation(programID,"View");
    GLuint ViewEntity = glGetUniformLocation(programID_Entity,"View");
    GLuint ProjectionMatrix = glGetUniformLocation(programID,"Projection");
    GLuint ProjectionEntity = glGetUniformLocation(programID_Entity,"Projection");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ParamsWindow *imgui = new ParamsWindow(window, 0, terrainControler); 

    // Chargement des textures
    GLint atlasTexture = loadTexture2DFromFilePath("../Textures/Blocks/atlas.png");
    GLint hudTexture = loadTexture2DFromFilePath("../Textures/HUD/hud.png");
    GLint entityTexture = loadTexture2DFromFilePath("../Textures/Entity/entity.png");

    glUseProgram(programID);
    if (atlasTexture != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTexture);
        glUniform1i(glGetUniformLocation(programID, "atlasTexture"), GL_TEXTURE0);
    }

    glUseProgram(programID_HUD);
    if (hudTexture != -1 && atlasTexture != -1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, hudTexture);
        glUniform1i(glGetUniformLocation(programID_HUD, "atlasTexture"), GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(programID_HUD, "hudTexture"), 1);
    }

    glUseProgram(programID_Entity);
    if (entityTexture != -1) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, entityTexture);
        glUniform1i(glGetUniformLocation(programID_Entity, "entityTexture"), 2);
    }

    skybox->bindCubemap(GL_TEXTURE3, 3); 

    lastFrame = glfwGetTime(); // Si on ne fait pas ça, le joueur tombe beaucoup trop vite à la première frame

    // Temporaire : Création des entités
    Entity *zombie = new Entity(0, 1,glm::vec3(3,1.4,3), 3.0f);
    zombie->loadEntity();
    Entity *cochon = new Entity(1, 1,glm::vec3(5,1.4,3), 1.0f);
    zombie->loadEntity();
    cochon->loadEntity();

    // Boucle de rendu
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glm::vec3 bottomPointPlayer = hitboxPlayer->getBottomPoint();

        // Courir consomme de l'endurance
        if (isRunning){
            if (player->getStamina() > 0.0){
                if (!modeJeu){ // En mode survie seulement
                    player->addStamina(-30.0*deltaTime);
                    hud->updateStamina(player->getStamina());
                }
                FoV = FoV_running;
            }else{
                player->applyAcceleration(false);
                isRunning = false;
                isHoldingShift = true;
            }
        }else if (modeJeu || player->getStamina() < 100.0f){ // On teste si on est en mode créatif, pour pouvoir revenir à la FoV normale si on ne court plus
            if (!modeJeu && !isHoldingShift){ // En mode survie seulement
                player->addStamina(30.0*deltaTime);
                hud->updateStamina(player->getStamina());
            }
            FoV = 75.0f;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 Projection = glm::perspective(glm::radians(FoV), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,2000.0f);

        if (cameraOrbitale){
            camera_target = -1.0f * camera_position;
        }else if (cameraMousePlayer){
            camera_position = bottomPointPlayer + glm::vec3(0.0f,1.7f,0.f); // Positionne la caméra sur le joueur (du coup attention à la taille qu'on donne à la hitbox)
        }

        glm::mat4 View = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        glUniformMatrix4fv(ModelMatrix,1,GL_FALSE,&Model[0][0]);
        glUniformMatrix4fv(ViewMatrix,1,GL_FALSE,&View[0][0]);
        glUniformMatrix4fv(ProjectionMatrix,1,GL_FALSE,&Projection[0][0]);

        terrainControler->drawTerrain();

        terrainControler->checkHoldLeftClick(camera_position, camera_target, deltaTime, modeJeu, programID);

        // Affichage de la skybox
        skybox->drawSkybox(Model, Projection, View);

        if (!switchToEditor){
            // Gestion des collisions
            hasUpdate = false;
            hitboxPlayer->checkJump(&hasUpdate, deltaTime);
            float damagePlayer = hitboxPlayer->checkTopAndBottomCollision(hasUpdate, deltaTime, terrainControler);
            if (!modeJeu && damagePlayer != 0.0f){ // En mode survie uniquement
                player->takeDamage(damagePlayer);
                hud->updateLife(player->getLife());
                if (player->getLife() <= 0.0){
                    std::cout << "Vous êtes mort !\n";
                    break; // Le joueur est mort, le programme s'arrête (en faisant attention à bien nettoyer la mémoire)
                }
            }
        }

        if (!switchToEditor){
            glUseProgram(programID_Entity);

            // Temporaire --------------------------------------------------------------------
            if(walkZombie){
                zombie->walk(zombie->getRootNode(),angleZombie,deltaTime);
                angleZombie += 6*deltaTime;
            }else{
                zombie->reset(zombie->getRootNode());
            }

            if (walkCochon){
                cochon->walkCochon(cochon->getRootNode(), angleCochon, deltaTime);
                angleCochon += 6*deltaTime;
            }else{
                cochon->reset(cochon->getRootNode());
            }

            if(fightZombie){
                zombie->attack(zombie->getRootNode(),&fightZombie,&accumulateurAnimation,deltaTime);
            }

            if(dieZombie){
                zombie->die(zombie->getRootNode(),&dieZombie,&accumulateurAnimation,deltaTime);
            }
            // -------------------------------------------------------------------------------

            glUniformMatrix4fv(ViewEntity,1,GL_FALSE,&View[0][0]);
            glUniformMatrix4fv(ProjectionEntity,1,GL_FALSE,&Projection[0][0]);

            zombie->drawEntity(programID_Entity);
            cochon->drawEntity(programID_Entity);
        }

        // Affichage de l'hud (Attention : Ca doit être la dernière chose à afficher dans la boucle de rendue, pour que l'hud se retrouve au premier plan)
        if (showHud){
            glDisable(GL_DEPTH_TEST);
            glUseProgram(programID_HUD);
            hud->drawHud();
            glEnable(GL_DEPTH_TEST);
        }

        if (!switchToEditor && imgui->getInEditor()){
            cameraMouseLibre = true;
            cameraMousePlayer = false;
            cameraOrbitale = false;
            cameraLibre = false;

            switchToEditor = true;
            delete terrainControler; // On supprime l'ancien terrain (on perd donc les modfications faites dessus)
            terrainControler = new TerrainControler(); // On génère un unique chunk     
            // TRES IMPORTANT : C'est ça qui causait la segfault qui m'a fait perdre 4 heures
            // Comme l'instance de TerrainControler est delete, il faut faire attention à bien utiliser la nouvelle instance pour l'instance de ParamsWindow 
            imgui->attachNewTerrain(terrainControler); 
        }else if (switchToEditor && !(imgui->getInEditor())){
            switchToEditor = false;
            delete terrainControler; // On supprime le terrain du mode éditeur
            terrainControler = new TerrainControler(3, 3, 1, 3, 1000, 4, nomStructure); // On revient au terrain initiale
            imgui->attachNewTerrain(terrainControler); // TRES IMPORTANT : C'est ça qui causait la segfault qui m'a fait perdre 4 heures
            hitboxPlayer->resetCanTakeDamage(); // Si le joueur tombe de trop haut, il ne faut pas qu'il meurt au moment où on quitte le mode éditeur
        }
        
        if (isImGuiShow){
            imgui->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(programID);
    glDeleteProgram(programID_HUD);
    glDeleteVertexArrays(1, &VertexArrayID);

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine); // Attention à bien nettoyer la mémoire, sinon segfault

    delete imgui;
    delete terrainControler;
    delete skybox;
    delete hud;
    delete player;
    delete window_object;
    delete zombie;
    delete cochon;
    glfwTerminate();
    return 0;
}
