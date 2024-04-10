#include <Headers.hpp>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define RANGE 4

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// Caméra
glm::vec3 camera_position  = glm::vec3(0.0f, 5.0f, 5.0f);
glm::vec3 camera_target;
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

// Ces 3 tailles sont en nombre de chunk
int planeWidth = 3; // De 1 à 32
int planeLength = 3; // De 1 à 32
int planeHeight = 1; // De 1 à 8

Player *player;
float playerSpeed = 0.1f;

int typeChunk = 0; // Chunk plein par défaut (0), Chunk sinus (1)

std::vector<Chunk*> listeChunks;
void buildPlanChunks(/*GLubyte *texels, GLint widthTexture, GLint heightTexture*/){
    listeChunks.clear();
    for (int i = 0 ; i < planeWidth ; i++){
        for (int j = 0 ; j < planeLength ; j++){
            for (int k = 0 ; k < planeHeight ; k++){
                Chunk *c = new Chunk(glm::vec3((planeWidth*32)/2*(-1.f) + i*32,(planeHeight*32)/2*(-1.f) + k*32,(planeLength*32)/2*(-1.f) + j*32), typeChunk/*, texels, widthTexture, heightTexture*/); 
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
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            glm::vec3 motion = glm::vec3(camera_target[0],0.0f,camera_target[2]);
            motion=glm::normalize(motion)*playerSpeed;
            player->move(motion);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            glm::vec3 motion = glm::normalize(glm::cross(camera_target,camera_up));
            motion=motion*playerSpeed;
            player->move(-motion);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            glm::vec3 motion = glm::vec3(camera_target[0],0.0f,camera_target[2]);
            motion=glm::normalize(motion)*playerSpeed;
            player->move(-motion);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            glm::vec3 motion = glm::normalize(glm::cross(camera_target,camera_up));
            motion=motion*playerSpeed;
            player->move(motion);
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
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            camera_position += (camera_speed / 5.f) * camera_target;
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
            camera_position -= (camera_speed / 5.f) * camera_target;
        }
        // Monter/Descendre la caméra
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            camera_position += (camera_speed / 5.f) * camera_up;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
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
        for (int k = 1 ; k < RANGE+1 ; k++){
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
    }
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

    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("../shaders/vertexShader.vert", "../shaders/fragmentShader.frag");

    player = new Player(glm::vec3(-0.5f,30.0f,-0.5f));
    
    /*
    GLint heightmap = loadTexture2DFromFilePath("../Textures/heightmap.png");
    GLint widthTexture, heightTexture;
    GLubyte *texels;
    if (heightmap != -1) { // Pour l'instant on lit cette texture dans le CPU au lieu des shaders
		glBindTexture(GL_TEXTURE_2D, heightmap);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &widthTexture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &heightTexture);
        
        texels = new GLubyte[widthTexture * heightTexture * 4]; // Je ne comprends pas pourquoi mais il faut multiplier par 4 la taille nécessaire
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_INT, texels);
	}
    buildPlanChunks(texels, widthTexture, heightTexture);
    */
    buildPlanChunks();

    /*
    Skybox *sky = new Skybox(1000.0f,glm::vec3(-500.0f,-500.0f,-500.0f));
    sky->loadSkybox();
    */

    glUseProgram(programID);

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

    // Chargement des textures (temporaire, plus tard on utilisera un atlas)
    GLint grassID = loadTexture2DFromFilePath("../Textures/grass.png");
    GLint cobblestoneID = loadTexture2DFromFilePath("../Textures/cobblestone.png");
    GLint diamondID = loadTexture2DFromFilePath("../Textures/diamond.png");
    GLint woodID = loadTexture2DFromFilePath("../Textures/wood.png");
    GLint obsidianID = loadTexture2DFromFilePath("../Textures/obsidian.png");

    if (grassID != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassID);
        glUniform1i(glGetUniformLocation(programID, "grassText"), GL_TEXTURE0);
	}
    if (cobblestoneID != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cobblestoneID);
        glUniform1i(glGetUniformLocation(programID, "cobblestoneText"), 1);
	}
    if (diamondID != -1) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, diamondID);
        glUniform1i(glGetUniformLocation(programID, "diamondText"), 2);
	}
    if (woodID != -1) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, woodID);
        glUniform1i(glGetUniformLocation(programID, "woodText"), 3);
	}
    if (obsidianID != -1) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, obsidianID);
        glUniform1i(glGetUniformLocation(programID, "obsidianText"), 4);
	}
    
    // Boucle de rendu
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,1000.0f);

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
        //sky->drawSkybox(programID);
        
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
                // Quand le joueur est au sommet du chunk, il ne faut pas qu'il rentre dans le bloc (à voir si on peut pas mieux faire)
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

        ImGui::SliderInt("Vitesse caméra", &speedCam, 5, 50);

        ImGui::Spacing();

        ImGui::SliderFloat("Vitesse Joueur", &playerSpeed, 0.1, 1.0);

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

        if (ImGui::SliderInt("Longueur", &planeWidth, 1, 32)){
            buildPlanChunks(/*texels, widthTexture, heightTexture*/);
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Largeur", &planeLength, 1, 32)){
            buildPlanChunks(/*texels, widthTexture, heightTexture*/);
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Hauteur", &planeHeight, 1, 8)){
            buildPlanChunks(/*texels, widthTexture, heightTexture*/);
        }

        ImGui::Spacing();

        // Type 0 = Plein ; Type 1 = Sinus
        if (ImGui::SliderInt("Type de chunk", &typeChunk, 0, 1)){
            buildPlanChunks();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;


}