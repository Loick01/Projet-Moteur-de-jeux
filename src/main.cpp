#include <Headers.hpp>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

GLuint programID, programID_HUD,programID_Entity;

bool isImGuiShow = true;

// --------------------------------
// Temporaire
float accumulateurDestructionBlock = 0.0f;
bool mouseLeftClickHold = false;
int previousIndiceVoxel;
// --------------------------------

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

// Temporaire
float angle=0.0f;
bool walk=false;
bool fight=false;
bool die=false;
float accumulateurAnimation = 0.0f; // Va servir à faire animations indépendantes du nombre de frame

int blockInHotbar[9] = {23,29,1,11,12,13,20,26,33}; // Blocs qui sont dans la hotbar
int indexHandBlock = 0;
int handBlock = blockInHotbar[indexHandBlock]; // ID du block que le joueur est en train de poser (se modifie à la molette de la souris)

bool showHud = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ 
    glViewport(0,0,width,height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_F && action == GLFW_PRESS){
        isImGuiShow = !isImGuiShow;
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
    }

    // ---------------------------------------------------------
    // Joue les animations du zombie (temporaire, on utilisera une classe Agent)
    // Marche
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){ 
            walk = !walk;
    }
    // Attaque
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){ 
            fight = !fight;
    }

    // Meurt
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){ 
            die = !die;
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
            if (hitboxPlayer->getLateralMovePossible(false,0.3f, bottomPlayer, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::vec3(camera_target[0],0.0f,camera_target[2]);
                z_axis = true;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ // Déplacement vers la gauche
            if (hitboxPlayer->getLateralMovePossible(true,-0.3f, bottomPlayer, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::normalize(cross_point);
                x_axis = true;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ // Déplacement en arrière
            if (hitboxPlayer->getLateralMovePossible(false,-0.3f, bottomPlayer, camera_target, camera_up, terrainControler, &cross_point)){
                motion += glm::vec3(camera_target[0],0.0f,camera_target[2])*-1.0f;
                z_axis = !z_axis;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ // Déplacement vers la droite
            if (hitboxPlayer->getLateralMovePossible(true,0.3f, bottomPlayer, camera_target, camera_up, terrainControler, &cross_point)){
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
        mouseLeftClickHold = true;
        LocalisationBlock lb = terrainControler->tryBreakBlock(camera_target, camera_position);
        previousIndiceVoxel = lb.indiceVoxel; // On conserve l'indice du voxel au moment où on clique
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        mouseLeftClickHold = false;
        accumulateurDestructionBlock = 0.0f;
    }else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        terrainControler->tryCreateBlock(camera_target, camera_position, handBlock);
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

    terrainControler = new TerrainControler(3, 3, 1, 3, 1000, 4);
    player = new Player(glm::vec3(-0.5f,10.0f,-0.5f),6.0f, 1.5f);
    hitboxPlayer = player->getHitbox();

    Skybox *skybox = new Skybox();

    glUseProgram(programID_HUD);
    Hud *hud = new Hud(SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform1i(glGetUniformLocation(programID_HUD, "selectLocation"), indexHandBlock);
    glUniform1iv(glGetUniformLocation(programID_HUD, "blockHotbar"), 9, blockInHotbar);
    hud->loadHud();

    GLuint ModelMatrix = glGetUniformLocation(programID,"Model");
    // La matrice Model ne sera pas envoyé aux shaders des entités (ça ne sert à rien)
    GLuint ViewMatrix = glGetUniformLocation(programID,"View");
    GLuint ViewEntity = glGetUniformLocation(programID_Entity,"View");
    GLuint ProjectionMatrix = glGetUniformLocation(programID,"Projection");
    GLuint ProjectionEntity = glGetUniformLocation(programID_Entity,"Projection");

    bool renduFilaire = false;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsLight();

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
    Zombie *zombie = new Zombie(1,glm::vec3(3,1.4,3), 3.0f);
    zombie->loadZombie();

    // Boucle de rendu
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (mouseLeftClickHold){
            LocalisationBlock lb = terrainControler->tryBreakBlock(camera_target, camera_position);
            // On part du principe que c'est impossible pour le joueur de viser un bloc d'un chunk à la frame n, puis le bloc équivalent d'un chunk adjacent à la frame n+1
            // Puisque la portée de son coup est limité à 4 (RANGE dans TerrainControler.hpp)
            // Du coup, il y a certain test qu'on peut se permettre d'éviter
            if (lb.indiceVoxel == previousIndiceVoxel){
                accumulateurDestructionBlock += deltaTime;
            }else if (lb.indiceVoxel != -1){
                previousIndiceVoxel = lb.indiceVoxel;
                accumulateurDestructionBlock = 0.0f;
            }else{
                accumulateurDestructionBlock = 0.0f;
            }

            if (accumulateurDestructionBlock >= 1.0f){
                terrainControler->breakBlock(lb);
                accumulateurDestructionBlock = 0.0f;
                // Si on voulait être précis, ici il aurait fallu remettre à jour previousIndiceBlock en rappelant tryBreakBlock
                // Mais ce n'est pas nécéssaire, ce sera fait seulement avec une frame de retard (pas trop grave)
            }
            //std::cout << "Accumulateur = " << accumulateurDestructionBlock << "\n";
        }

        glm::vec3 bottomPointPlayer = hitboxPlayer->getBottomPoint();

        // Courir consomme de l'endurance
        if (isRunning){
            if (player->getStamina() > 0.0){
                player->addStamina(-30.0*deltaTime);
                hud->updateStamina(player->getStamina());
                FoV = FoV_running;
            }else{
                player->applyAcceleration(false);
                isRunning = false;
                isHoldingShift = true;
            }
        }else if (player->getStamina() < 100.0f){
            if (!isHoldingShift){
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

        // Affichage de la skybox
        skybox->drawSkybox(Model, Projection, View);

        // Gestion des collisions
        hasUpdate = false;
        hitboxPlayer->checkJump(&hasUpdate, deltaTime);
        hitboxPlayer->checkTopAndBottomCollision(hasUpdate, deltaTime, terrainControler, hud, player);

        glUseProgram(programID_Entity);

        // Temporaire --------------------------------------------------------------------
        if(walk==true){
            zombie->walk(zombie->getRootNode(),angle,deltaTime);
            angle += 6*deltaTime;
        }else{
            zombie->reset(zombie->getRootNode());
        }

        if(fight==true){
            zombie->attack(zombie->getRootNode(),&fight,&accumulateurAnimation,deltaTime);
        }

        if(die==true){
            zombie->die(zombie->getRootNode(),&die,&accumulateurAnimation,deltaTime);
        }
        // -------------------------------------------------------------------------------

        glUniformMatrix4fv(ViewEntity,1,GL_FALSE,&View[0][0]);
        glUniformMatrix4fv(ProjectionEntity,1,GL_FALSE,&Projection[0][0]);

        zombie->drawZombie(programID_Entity);

        // Affichage de l'hud (Attention : Ca doit être la dernière chose à afficher dans la boucle de rendue, pour que l'hud se retrouve au premier plan)
        if (showHud){
            glDisable(GL_DEPTH_TEST);
            glUseProgram(programID_HUD);
            hud->drawHud();
            glEnable(GL_DEPTH_TEST);
        }
        
        if (isImGuiShow){
            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            float fps = ImGui::GetIO().Framerate;

            ImGui::NewFrame();
            ImGui::Begin("Panneau de contrôle");
            ImGui::Text("FPS : %.2f", fps);

            ImGui::Spacing();

            ImGui::Text("Position : %.2f / %.2f / %.2f", bottomPointPlayer[0], bottomPointPlayer[1], bottomPointPlayer[2]);

            ImGui::Spacing();

            ImGui::SliderInt("Vitesse caméra", &speedCam, 5, 100);

            ImGui::Spacing();

            //ImGui::SliderFloat("Vitesse Joueur", &playerSpeed, 0.0, 50.0);

            ImGui::Spacing();

            ImGui::SliderFloat("FoV", &FoV, 1.0, 179.9);

            ImGui::Spacing();

            if (ImGui::Checkbox("Caméra orbitale", &cameraOrbitale)){
                cameraLibre = false;
                cameraMouseLibre = false;
                cameraMousePlayer = false;
            }

            ImGui::Spacing();
            
            if (ImGui::Checkbox("Caméra libre", &cameraLibre)){
                cameraOrbitale = false;
                cameraMouseLibre = false;
                cameraMousePlayer = false;
            }

            ImGui::Spacing();
            
            if (ImGui::Checkbox("Caméra souris", &cameraMouseLibre)){
                cameraLibre = false;
                cameraOrbitale = false;
                cameraMousePlayer = false;
            }

            if (ImGui::Checkbox("Caméra player", &cameraMousePlayer)){
                cameraLibre = false;
                cameraOrbitale = false;
                cameraMouseLibre = false;
            }

            ImGui::Spacing();

            if (ImGui::Checkbox("Rendu filaire", &renduFilaire)) {
                if (renduFilaire) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }

            ImGui::Spacing();

            ImGui::Checkbox("Afficher l'hud", &showHud);

            ImGui::Spacing();

            /*
            if (ImGui::SliderInt("Longueur terrain", &planeWidth, 1, 32)){
                mg->setWidthMap(planeWidth);
                mg->generateImage();
                dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
                buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
            }
            */

            ImGui::Spacing();

            /*
            if (ImGui::SliderInt("Largeur", &planeLength, 1, 32)){
                mg->setHeightMap(planeLength);
                mg->generateImage();
                dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
                buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
            }
            */

            ImGui::Spacing();

            /*
            if(ImGui::SliderInt("Seed de génération", &seedTerrain, 0, 10000)){
                mg->setSeed(seedTerrain);
            }
            */

            ImGui::Spacing();

            /*
            if(ImGui::SliderInt("Nombre d'octaves", &octave, 1, 10)){
                mg->setOctave(octave);
            }
            */

            ImGui::Spacing();

            /*
            if (ImGui::Button("Utiliser la seed et le nombre d'octaves")){
                mg->generateImage();
                dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
                buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
            }
            */

            // Pour simplifier, on limite à un seul chunk de haut
            //if (ImGui::SliderInt("Hauteur", &planeHeight, 1, 8)){
            //    buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
            //}

            ImGui::Spacing();

            /*
            // Type 0 = Plein ; Type 1 = Sinus ; Type 2 = Flat ; Type 3 = Procedural
            if (ImGui::SliderInt("Type de chunk", &typeChunk, 0, 3)){
                buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
            }
            */

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(programID);
    glDeleteProgram(programID_HUD);
    glDeleteVertexArrays(1, &VertexArrayID);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete terrainControler;

    delete skybox;
    delete hud;
    delete player;
    delete window_object;
    delete zombie;
    glfwTerminate();
    return 0;
}
