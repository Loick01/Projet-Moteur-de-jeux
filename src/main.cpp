#include <Headers.hpp>

// Variables partagées avec la fenêtre ImGui
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
int speedCam = 60;
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

int blockInHotbar[9] = {0,1,2,3,4,5,6,7,8}; // Blocs qui sont dans la hotbar
int indexHandBlock = 0;
int handBlock = blockInHotbar[indexHandBlock]; // ID du block que le joueur est en train de poser (se modifie à la molette de la souris)

bool showHud = true;
int isShadow = 1; // 1 si on utilise les ombres dans le shader, 0 sinon
bool modeJeu = false; // true pour créatif, false pour survie

// Cette objet permet de lancer tous les sons qui seront nécéssaire
Sound *soundManager;

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

    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS){
        if (blockInHotbar[8]<36){
            for (int i = 0 ; i < 9 ; i++){
                blockInHotbar[i] += 1;
            }
        }
        handBlock = blockInHotbar[indexHandBlock];
        glUniform1iv(glGetUniformLocation(programID_HUD, "blockHotbar"), 9, blockInHotbar); // On envoie la nouvelle hotbar aux shaders
    }
    if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS){
        if (blockInHotbar[0]>0){
            for (int i = 0 ; i < 9 ; i++){
                blockInHotbar[i] -= 1;
            }
        }
        handBlock = blockInHotbar[indexHandBlock];
        glUniform1iv(glGetUniformLocation(programID_HUD, "blockHotbar"), 9, blockInHotbar); // On envoie la nouvelle hotbar aux shaders
    }

    // Pour sortir de la caméra à la souris et avoir le curseur
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
                hitboxPlayer->resetJumpForce(1);
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
        if (terrainControler->tryCreateBlock(camera_target, camera_position, handBlock)){
            soundManager->playCreateSound();
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    indexHandBlock = (indexHandBlock + (yoffset > 0 ? -1 : 1) + 9) % 9; // 9 emplacements dans la hotbar
    glUniform1i(glGetUniformLocation(programID_HUD, "selectLocation"), indexHandBlock);
    handBlock = blockInHotbar[indexHandBlock];
}

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

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    programID = LoadShaders("../shaders/vertexShader.vert", "../shaders/fragmentShader.frag");
    programID_HUD = LoadShaders("../shaders/hud_vertex.vert", "../shaders/hud_frag.frag");
    programID_Entity = LoadShaders("../shaders/entity_vertex.vert", "../shaders/entity_frag.frag");

    std::vector<std::string> structureBiome0;
    structureBiome0.push_back("../Structures/Tree.txt");
    structureBiome0.push_back("../Structures/Tree_2.txt");
    structureBiome0.push_back("../Structures/Tree_3.txt");
    structureBiome0.push_back("../Structures/House_1.txt");

    std::vector<std::string> structureBiome1;
    structureBiome1.push_back("../Structures/arche.txt");
    structureBiome1.push_back("../Structures/cactus_amas.txt");
    structureBiome1.push_back("../Structures/cactus_simple_1.txt");
    structureBiome1.push_back("../Structures/cactus_simple_2.txt");

    std::vector<std::string> structureBiome2;
    structureBiome2.push_back("../Structures/Glace.txt");
    structureBiome2.push_back("../Structures/Sapin.txt");

    std::vector<std::vector<std::string>> nomStructure;

    nomStructure.push_back(structureBiome0);
    nomStructure.push_back(structureBiome1);
    nomStructure.push_back(structureBiome2);

    terrainControler = new TerrainControler(12, 12, 1, 3, 1000, 4, nomStructure);
    player = new Player(glm::vec3(-0.5f,10.0f,-0.5f), 1.8f, 0.6f, 6.0f, 1.5f); // Le joueur fait 1.8 bloc de haut, et 0.6 bloc de large et de long
    hitboxPlayer = player->getHitbox();

    Skybox *skybox = new Skybox();

    soundManager = new Sound();
    soundManager->playRandomBackground();

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

    ParamsWindow *imgui = new ParamsWindow(window, 0, terrainControler, player); 

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

    std::vector<Entity*> listeEntity;
    for (int i = 0 ; i < 0 ; i++){
        listeEntity.push_back(new Entity(0, 1,glm::vec3(i*0.05f,32.0,3), 3.0f,2.1f,0.5f,0.5f, 6.0, 6.0, 10.0, 1.0));
        listeEntity[i]->loadEntity();
    }

    bool playerDie = false;

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
            camera_target = glm::normalize(camera_target);
        }else if (cameraMousePlayer){
            camera_position = bottomPointPlayer + glm::vec3(0.0f,1.7f,0.f); // Positionne la caméra sur le joueur (du coup attention à la taille qu'on donne à la hitbox)
        }

        glm::mat4 View = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        glUniformMatrix4fv(ModelMatrix,1,GL_FALSE,&Model[0][0]);
        glUniformMatrix4fv(ViewMatrix,1,GL_FALSE,&View[0][0]);
        glUniformMatrix4fv(ProjectionMatrix,1,GL_FALSE,&Projection[0][0]);

        terrainControler->drawTerrain();

        if (terrainControler->checkHoldLeftClick(camera_position, camera_target, deltaTime, modeJeu, programID)){
            soundManager->playBreakSound();
        }

        // Affichage de la skybox
        skybox->drawSkybox(Model, Projection, View);

        if (!switchToEditor){
            // Gestion des collisions
            hasUpdate = false;
            hitboxPlayer->checkJump(&hasUpdate, deltaTime);
            float damagePlayer = hitboxPlayer->checkTopAndBottomCollision(hasUpdate, deltaTime, terrainControler);
            if (damagePlayer == -1.0){
                playerDie = true;
            }else if (!modeJeu && damagePlayer > 0.0f){ // En mode survie uniquement
                player->takeDamage(damagePlayer);
                hud->updateLife(player->getLife());
                if (player->getLife() <= 0.0){
                    std::cout << "Vous êtes mort !\n";
                    playerDie = true; // On va sortir de la boucle de jeu, et correctement nettoyer la mémoire
                }
            }
        }

        if (!switchToEditor){
            glUseProgram(programID_Entity);
            glUniformMatrix4fv(ViewEntity,1,GL_FALSE,&View[0][0]);
            glUniformMatrix4fv(ProjectionEntity,1,GL_FALSE,&Projection[0][0]);

            for (int i = 0 ; i < listeEntity.size() ; i++){
                if (listeEntity[i] != nullptr){
                    float damage = listeEntity[i]->drawEntity(programID_Entity, listeEntity[i]->getType(), deltaTime,terrainControler,player);
                    if (!modeJeu && damage != 0.0f){ // En mode survie uniquement
                        player->takeDamage(damage);
                        hud->updateLife(player->getLife());
                        if (player->getLife() <= 0.0){
                            std::cout << "Vous êtes mort !\n";
                            playerDie = true; // On va sortir de la boucle de jeu, et correctement nettoyer la mémoire
                        }
                    }
                    if (listeEntity[i]->getLife() <= 0.0){
                        delete listeEntity[i];
                        listeEntity[i] = nullptr;
                    }
                }
            }
        }

        if (playerDie){
            break; // On sort de la boucle de jeu
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
            terrainControler = new TerrainControler(12, 12, 1, 3, 1000, 4, nomStructure); // On revient au terrain initiale
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

    delete imgui;
    delete terrainControler;
    delete skybox;
    delete hud;
    delete player;
    delete window_object;
    for (int i = 0 ; i < listeEntity.size() ; i++){
        if (listeEntity[i] != nullptr){
            delete listeEntity[i];
        }
    }
    glfwTerminate();
    return 0;
}
