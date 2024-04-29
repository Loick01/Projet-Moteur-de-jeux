#include <Headers.hpp>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define RANGE 4

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

GLuint programID, programID_HUD;

// Caméra
glm::vec3 camera_position  = glm::vec3(0.0f, 5.0f, 5.0f);
glm::vec3 camera_target = glm::vec3(1.0f,0.0f,-1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f,  0.0f);

bool cameraOrbitale = false;
bool cameraLibre = false; // Caméra libre par défaut
bool cameraMouseLibre = false;
bool cameraMousePlayer = true;
int speedCam = 15;
double previousX = SCREEN_WIDTH / 2;
double previousY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
float phi = -90.0f;
float theta = 0.0f;
float FoV = 45.0f;

// Ces 3 tailles sont en nombre de chunk
int planeWidth = 3; // De 1 à 32
int planeLength = 3; // De 1 à 32
int planeHeight = 1; // De 1 à 8

Player *player;
float playerSpeed = 6.0f;

int blockInHotbar[9] = {23,29,1,11,12,13,20,26,28}; // Blocs qui sont dans la hotbar
int indexHandBlock = 0;
int handBlock = blockInHotbar[indexHandBlock]; // ID du block que le joueur est en train de poser (se modifie à la molette de la souris)

bool showHud = true;
int typeChunk = 3; // Chunk plein (0), Chunk sinus (1), Chunk plat (2), Chunk procédural (3)

std::vector<Chunk*> listeChunks;
void buildPlanChunks(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap){
    listeChunks.clear();
    for (int i = 0 ; i < planeWidth ; i++){
        for (int j = 0 ; j < planeLength ; j++){
            for (int k = 0 ; k < planeHeight ; k++){
                Chunk *c = new Chunk(glm::vec3((planeWidth*32)/2*(-1.f) + i*32,(planeHeight*32)/2*(-1.f) + k*32,(planeLength*32)/2*(-1.f) + j*32), typeChunk, dataPixels, widthHeightmap, heightHeightmap, i*32,j*32*planeWidth*32); 
                c->loadChunk();
                listeChunks.push_back(c);
            }
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ 
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){ 
    float camera_speed = (float)speedCam * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }

    // Pour sortir de la caméra à la souris (plus tard ce sera la touche qui ouvre l'inventaire, et donc affiche la souris dans la fenêtre)
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){ 
            cameraMouseLibre = false;
            cameraMousePlayer = false;
            cameraOrbitale = false;
            cameraLibre = true;
    }
    
    // Déplacement du joueur
    if(cameraMousePlayer){
        bool x_axis = false;
        bool z_axis = false;
        // On est obligé de vérifier que le joueur n'appuie pas sur 2 touches opposées en même temps (sinon motion est nulle et ça fait des valeurs NaN)

        glm::vec3 motion = glm::vec3(0.0f,0.0f,0.0f); // On accumule les déplacements pour que le joueur puisse se déplacer en diagonale
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            motion += glm::vec3(camera_target[0],0.0f,camera_target[2]);
            z_axis = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            motion += glm::cross(camera_target,camera_up)*-1.0f;
            x_axis = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            motion += glm::vec3(camera_target[0],0.0f,camera_target[2])*-1.0f;
            z_axis = !z_axis;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            motion += glm::cross(camera_target,camera_up);
            x_axis = !x_axis;
        }
        if (x_axis || z_axis){
            player->move(glm::normalize(motion)*playerSpeed*deltaTime); // Attention à bien normaliser le vecteur de déplacement final (ça règle le problème de sqrt(2))
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            // On initie le saut du joueur
            if (player->getCanJump()){
                player->couldJump(false);
                player->addToSpeed(0.23f);
                player->move(glm::vec3(0.f,0.23f,0.f));
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
        glm::vec3 originPoint = camera_position;
        glm::vec3 direction = normalize(camera_target);
        for (int k = 1 ; k < RANGE+1 ; k++){ // Trouver une meilleure manière pour détecter le bloc à casser
        // for (float k = 0.1 ; k < RANGE+1. ; k+=0.1){
            glm::vec3 target = originPoint + (float)k*direction;
            int numLongueur = floor(target[0]) + 16*planeWidth;
            int numHauteur = floor(target[1]) + 16;
            int numProfondeur = floor(target[2]) + 16*planeLength;
            if (numLongueur < 0 || numLongueur > (planeWidth*32)-1 || numProfondeur < 0 || numProfondeur > (planeLength*32)-1 || numHauteur < 0 || numHauteur > 31){
                continue; // Attention à ne pas mettre return même si c'est tentant (par exemple si le joueur regarde vers le bas en étant au sommet d'un chunk)
            }else{
                int indiceV = numHauteur *1024 + (numProfondeur%32) * 32 + (numLongueur%32); // Indice du voxel que le joueur est en train de viser
                int indiceChunk = (numLongueur/32) * planeLength + numProfondeur/32;
                std::vector<Voxel*> listeVoxels = listeChunks[indiceChunk]->getListeVoxels();

                if (listeVoxels[indiceV] == nullptr){
                    continue;
                }else{
                    listeVoxels[indiceV] = nullptr;

                    // Rendre visible les 6 cubes adjacents (s'ils existent et s'ils ne sont pas déjà visible)
                    // Il faudrait chercher une meilleure façon de faire ça
                    for (int c = -1 ; c < 2 ; c+=2){
                        int numLongueurVoisin = (numLongueur%32) + c;
                        int numHauteurVoisin = numHauteur + c;
                        int numProfondeurVoisin = (numProfondeur%32) + c;
                        int indiceVoisin;

                        if (numLongueurVoisin >= 0 && numLongueurVoisin <= 31){
                            indiceVoisin = numHauteur *1024 + (numProfondeur%32) * 32 + numLongueurVoisin;
                            if (listeVoxels[indiceVoisin] != nullptr && !(listeVoxels[indiceVoisin]->getVisible())){ // On vérifie si le voxel n'est pas déjà visible (en vrai c'est pas obligatoire)
                                listeVoxels[indiceVoisin]->setVisible(true);
                            }
                        }
                        if (numHauteurVoisin >= 0 && numHauteurVoisin <= 31){
                            indiceVoisin = numHauteurVoisin *1024 + (numProfondeur%32) * 32 + (numLongueur%32);
                            if (listeVoxels[indiceVoisin] != nullptr && !(listeVoxels[indiceVoisin]->getVisible())){
                                listeVoxels[indiceVoisin]->setVisible(true);
                            }
                        }
                        if (numProfondeurVoisin >= 0 && numProfondeurVoisin <= 31){
                            indiceVoisin = numHauteur *1024 + numProfondeurVoisin * 32 + (numLongueur%32);
                            if (listeVoxels[indiceVoisin] != nullptr && !(listeVoxels[indiceVoisin]->getVisible())){
                                listeVoxels[indiceVoisin]->setVisible(true);
                            }
                        }
                    }

                    listeChunks[indiceChunk]->setListeVoxels(listeVoxels);
                    listeChunks[indiceChunk]->loadChunk();
                    return;
                }
            }
        }
    }else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        glm::vec3 originPoint = camera_position;
        glm::vec3 direction = normalize(camera_target);
        float k = 3.0; // Pour l'instant le joueur ne peut poser un block qu'à cette distance
        glm::vec3 target = originPoint + (float)k*direction;
        int numLongueur = floor(target[0]) + 16*planeWidth;
        int numHauteur = floor(target[1]) + 16;
        int numProfondeur = floor(target[2]) + 16*planeLength;
        if (numLongueur < 0 || numLongueur > (planeWidth*32)-1 || numProfondeur < 0 || numProfondeur > (planeLength*32)-1 || numHauteur < 0 || numHauteur > 31){
            return;
        }else{
            int indiceV = numHauteur *1024 + (numProfondeur%32) * 32 + (numLongueur%32); // Indice du voxel que le joueur est en train de viser
            int indiceChunk = (numLongueur/32) * planeLength + numProfondeur/32;
            std::vector<Voxel*> listeVoxels = listeChunks[indiceChunk]->getListeVoxels();

            if (listeVoxels[indiceV] == nullptr){
                glm::vec3 posChunk = listeChunks[indiceChunk]->getPosition();
                Voxel* vox = new Voxel(glm::vec3(posChunk[0]+numLongueur%32,posChunk[1]+numHauteur,posChunk[2]+numProfondeur%32),handBlock);
                vox->setVisible(true);
                listeVoxels[indiceV] = vox;

                // A voir si ici on ne pourrait pas mettre à jour la visibilité de certains voxels pour faire mieux

                listeChunks[indiceChunk]->setListeVoxels(listeVoxels);
                listeChunks[indiceChunk]->loadChunk();
            }
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
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window_object(3, 3, SCREEN_WIDTH, SCREEN_HEIGHT, "Projet Moteur de jeux");
    window_object.setup_GLFW();
    GLFWwindow* window = window_object.get_window();
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
    

    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    programID = LoadShaders("../shaders/vertexShader.vert", "../shaders/fragmentShader.frag");
    programID_HUD = LoadShaders("../shaders/hud_vertex.vert", "../shaders/hud_frag.frag");
    glUseProgram(programID_HUD); // Attention à bien laisser cette ligne (apparemment il faut un glUseProgram initialement sinon ça cause des problèmes quand on essaye de charger des textures)

    player = new Player(glm::vec3(-0.5f,10.0f,-0.5f));

    MapGenerator *mg = new MapGenerator(planeWidth, planeLength, 21345); 
    mg->generateImage();
    int widthHeightmap, heightHeightmap, channels;
    unsigned char* dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);

    if (widthHeightmap != planeWidth*32 || heightHeightmap != planeLength*32 ){ // On s'assure que la carte de hauteur chargée est bien adapté au terrain
        std::cout << "La carte de hauteur n'est pas adapté au terrain\n";
        return -1;
    }

    buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);

    Skybox *skybox = new Skybox();

    Hud *hud = new Hud(SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform1i(glGetUniformLocation(programID_HUD, "selectLocation"), indexHandBlock);
    hud->loadHud();

    GLuint ModelMatrix = glGetUniformLocation(programID,"Model");
    GLuint ViewMatrix = glGetUniformLocation(programID,"View");
    GLuint ProjectionMatrix = glGetUniformLocation(programID,"Projection");

    bool renduFilaire = false;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsLight();

    // Temporaire (plus tard il faudra envoyer aux shaders le tableau des blocs dans la hotbar à chaque fois que celle-ci changera)
    glUniform1iv(glGetUniformLocation(programID_HUD, "blockHotbar"), 9, blockInHotbar);

    // Chargement des textures
    GLint atlasTexture = loadTexture2DFromFilePath("../Textures/Blocks/atlas.png");
    GLint hudTexture = loadTexture2DFromFilePath("../Textures/HUD/hud.png");

    if (atlasTexture != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTexture);
        glUniform1i(glGetUniformLocation(programID, "atlasTexture"), GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(programID_HUD, "atlasTexture"), GL_TEXTURE0);
    }

    if (hudTexture != -1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, hudTexture);
        glUniform1i(glGetUniformLocation(programID_HUD, "hudTexture"), 1);
    }

    skybox->bindCubemap(GL_TEXTURE2, 2); 
    
    // Boucle de rendu
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 Projection = glm::perspective(glm::radians(FoV), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,2000.0f);

        if (cameraOrbitale){
            camera_target = -1.0f * camera_position;
        }else if (cameraMousePlayer){
            camera_position = player->getBottomPoint() + glm::vec3(0.0f,1.8f,0.f); // Positionne la caméra sur le joueur
        }

        glm::mat4 View = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        glUniformMatrix4fv(ModelMatrix,1,GL_FALSE,&Model[0][0]);
        glUniformMatrix4fv(ViewMatrix,1,GL_FALSE,&View[0][0]);
        glUniformMatrix4fv(ProjectionMatrix,1,GL_FALSE,&Projection[0][0]);


        for (int i = 0 ; i < listeChunks.size() ; i++){
            listeChunks[i]->drawChunk();
        }

        // Affichage de la skybox
        skybox->drawSkybox(Model, Projection, View);

        // Affichage de l'hud
        if (showHud){
            glUseProgram(programID_HUD);
            hud->drawHud();
        }

        // Pour les collisions, voir peut être swept aabb
        // Détermine la cellule ou se trouve le joueur
        glm::vec3 pPlayer = player->getBottomPoint();
        int numLongueur = floor(pPlayer[0]) + 16*planeWidth;
        int numHauteur = floor(pPlayer[1]-0.001) + 16;
        int numProfondeur = floor(pPlayer[2]) + 16*planeLength;
        if (numLongueur < 0 || numLongueur > (planeWidth*32)-1 || numProfondeur < 0 || numProfondeur > (planeLength*32)-1 || numHauteur < 0 || numHauteur > 31){
            player->move(glm::vec3(0.f,player->getJumpSpeed(),0.f));
            player->addToSpeed(-0.02);
        }else{
            int indiceBlock = numHauteur *1024 + (numProfondeur%32) * 32 + (numLongueur%32); // Indice du voxel dans lequel on considère que le joueur se trouve
            Voxel *v = listeChunks[(numLongueur/32) * planeLength + numProfondeur/32]->getListeVoxels()[indiceBlock];
            Voxel *vLeft = listeChunks[(numLongueur/32) * planeLength + numProfondeur/32]->getListeVoxels()[indiceBlock+1023];
            // glm::vec3 pLeftBottomPlayer=player->getLeftBottomPoint();
            // int numLongueur2 = floor(pPlayer[0]) + 16*planeWidth;
            // int numHauteur2 = floor(pPlayer[1]-0.001) + 16;
            // int numProfondeur2 = floor(pPlayer[2]) + 16*planeLength;
            
            printf("indice: %d\n",indiceBlock);
            printf("indice gauche: %d\n",indiceBlock+1023);
            if(player->getCanJump()==false){
                Voxel *vTop = listeChunks[(numLongueur/32) * planeLength + numProfondeur/32]->getListeVoxels()[indiceBlock+1024*2];
                if(vTop != nullptr){
                    //printf("block au dessus \n");
                    //if(pTopPlayer[1]==vTopPos[1])printf("collision\n");
                    player->couldJump(false);
                    float actualJumpSpeed=player->getJumpSpeed();
                    player->resetJumpSpeed();
                    player->move(glm::vec3(0.f,-0.01,0.f));
                    //player->addToSpeed(-2*actualJumpSpeed);
                }
            }

            if(vLeft!=nullptr){
                printf("il y'a un bloc a gauche\n");
                glm::vec3 pos = vLeft->backBottomLeftCorner;
            }
            
            if (v == nullptr){
                player->move(glm::vec3(0.f,player->getJumpSpeed(),0.f));
                player->addToSpeed(-0.02);

                // On fait attention à ne pas afficher le joueur à l'intérieur d'un bloc (même pendant une frame, c'est plus propre)
                pPlayer = player->getBottomPoint();
                numHauteur = floor(pPlayer[1]-0.001) + 16;
                indiceBlock = numHauteur *1024 + (numProfondeur%32) * 32 + (numLongueur%32);
                v = listeChunks[(numLongueur/32) * planeLength + numProfondeur/32]->getListeVoxels()[indiceBlock];
                if (v != nullptr){
                    player->move(glm::vec3(0.f,ceil(pPlayer[1]) - pPlayer[1],0.f));
                }
            }else{
                // Quand le joueur est au sommet du chunk, il ne faut pas qu'il rentre dans le bloc
                if (pPlayer[1] != ceil(pPlayer[1])){
                    player->move(glm::vec3(0.f,ceil(pPlayer[1]) - pPlayer[1],0.f));
                }
                player->resetJumpSpeed();
                player->couldJump(true);
            }
        }
        
        
        
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        float fps = ImGui::GetIO().Framerate;

        ImGui::NewFrame();
        ImGui::Begin("Panneau de contrôle");
        ImGui::Text("FPS : %.2f", fps);

        ImGui::Spacing();

        ImGui::Text("Position : %.2f / %.2f / %.2f", pPlayer[0], pPlayer[1], pPlayer[2]);

        ImGui::Spacing();

        ImGui::SliderInt("Vitesse caméra", &speedCam, 5, 100);

        ImGui::Spacing();

        ImGui::SliderFloat("Vitesse Joueur", &playerSpeed, 0.0, 100.0);

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

        if (ImGui::SliderInt("Longueur", &planeWidth, 1, 32)){
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Largeur", &planeLength, 1, 32)){
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Hauteur", &planeHeight, 1, 8)){
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }

        ImGui::Spacing();

        // Type 0 = Plein ; Type 1 = Sinus ; Type 2 = Flat ; Type 3 = Procedural
        if (ImGui::SliderInt("Type de chunk", &typeChunk, 0, 3)){
            buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(programID);
    glDeleteProgram(programID_HUD);
    glDeleteVertexArrays(1, &VertexArrayID);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;


}
