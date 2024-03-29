#include <Headers.hpp>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// Caméra
glm::vec3 camera_position  = glm::vec3(0.0f, 5.0f, 5.0f);
glm::vec3 camera_target = camera_position * -1.0f;
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f,  0.0f);
bool cameraOrbitale = false;
bool cameraLibre = false;
bool cameraPerso = false;
int speedCam = 50;
double previousX = SCREEN_WIDTH / 2;
double previousY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
float phi = -90.0f;
float theta = 0.0f;

// Ces 3 tailles sont en nombre de chunk
int planeWidth = 16; // De 1 à 32
int planeLength = 1; // De 1 à 32
int planeHeight = 1; // De 1 à 
//Personnage *personnage;

std::vector<Chunk*> listeChunks;
void buildPlanChunks(){
    listeChunks.clear();
    for (int i = 0 ; i < planeWidth ; i++){
        for (int j = 0 ; j < planeLength ; j++){
            for (int k = 0 ; k < planeHeight ; k++){
                Chunk *c = new Chunk(glm::vec3((planeWidth*32)/2*(-1.f) + i*32,(planeHeight*32)/2*(-1.f) + k*32,(planeLength*32)/2*(-1.f) + j*32)); 
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

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // Touche échap
        glfwSetWindowShouldClose(window,true);
    }

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
    
    /*
    // Déplacement du personnage
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        personnage->move(glm::vec3(0.f,0.f,-0.1f));
        personnage->loadPerso();
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        personnage->move(glm::vec3(-0.1f,0.f,0.f));
        personnage->loadPerso();
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        personnage->move(glm::vec3(0.f,0.f,0.1f));
        personnage->loadPerso();
    }
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS){
        personnage->move(glm::vec3(0.1f,0.f,0.f));
        personnage->loadPerso();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        if(personnage->getRepresentant()->getPoint()[1] == 1.0f){
            // On initie le saut du personnage
            personnage->setJumpSpeed(0.23);
            personnage->move(glm::vec3(0.f,personnage->getJumpSpeed(),0.f));
            personnage->loadPerso();
        }
    }
    */
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    //std::cout << "Changement : " << xpos << ", " << ypos << "\n";
    if (firstMouse){
        previousX = xpos;
        previousY = ypos;
        firstMouse = false;
    }

    double deltaX = xpos - previousX;
    double deltaY = previousY - ypos;

    phi += deltaX*0.05f;
    theta += deltaY*0.05f;

    float x = cos(glm::radians(phi)) * cos(glm::radians(theta));
    float y = sin(glm::radians(theta));
    float z = sin(glm::radians(phi)) * cos(glm::radians(theta));
    camera_target = glm::normalize(glm::vec3(x,y,z));

    previousX=xpos;
    previousY=ypos;
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
    glClearColor(0.05f, 0.06f, 0.22f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE); // Attention à la construction des triangles

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("../shaders/vertexShader.vert", "../shaders/fragmentShader.frag");

    /*
    personnage = new Personnage(glm::vec3(0.0f,2.0f,0.0f));
    personnage->loadPerso();
    */
    //Voxel *vox = new Voxel(glm::vec3(-0.5f,-0.5f,-0.5f)); 
    //vox->loadVoxel();
    /*
    Chunk *chunky = new Chunk(glm::vec3(-16.0f,-16.0f,-16.0f));
    chunky->loadChunk();
    */
    buildPlanChunks();

    Skybox *sky = new Skybox(1000.0f,glm::vec3(-500.0f,-500.0f,-500.0f));
    sky->loadSkybox();

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
    ImGui::StyleColorsDark();

    // Chargement des textures
    GLint nxGrass = loadTexture2DFromFilePath("../Textures/Grass/nx_grass.png");
    GLint pxGrass = loadTexture2DFromFilePath("../Textures/Grass/px_grass.png");
    GLint nyGrass = loadTexture2DFromFilePath("../Textures/Grass/ny_grass.png");
    GLint pyGrass = loadTexture2DFromFilePath("../Textures/Grass/py_grass.png");
    GLint nzGrass = loadTexture2DFromFilePath("../Textures/Grass/nz_grass.png");
    GLint pzGrass = loadTexture2DFromFilePath("../Textures/Grass/pz_grass.png");

    GLint skyTexture = loadTexture2DFromFilePath("../Textures/Skybox/nx.png");

    if (nxGrass != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nxGrass);
        glUniform1i(glGetUniformLocation(programID, "nxTexture"), GL_TEXTURE0);
	}
    if (pxGrass != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pxGrass);
        glUniform1i(glGetUniformLocation(programID, "pxTexture"), 1);
	}
    if (nyGrass != -1) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, nyGrass);
        glUniform1i(glGetUniformLocation(programID, "nyTexture"), 2);
	}
    if (pyGrass != -1) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, pyGrass);
        glUniform1i(glGetUniformLocation(programID, "pyTexture"), 3);
	}
    if (nzGrass != -1) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, nzGrass);
        glUniform1i(glGetUniformLocation(programID, "nzTexture"), 4);
	}
    if (pzGrass != -1) {
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, pzGrass);
        glUniform1i(glGetUniformLocation(programID, "pzTexture"), 5);
	}
    if (skyTexture != -1) {
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, skyTexture);
        glUniform1i(glGetUniformLocation(programID, "skyTexture"), 6);
	}

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Pour masquer la souris sur la fenêtre

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

        //camera_position = personnage->getRepresentant()->getPoint() + glm::vec3(0.5f,2.f,4.f);
        // On verra plus tard pour faire les changements de caméra au cours de l'éxécution
        /*
        if (cameraOrbitale){
            //glfwSetCursorPosCallback(window, NULL); // On met en pause le callback
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Pour remettre la souris si on l'avait enlevé
            camera_target = -1.0f * camera_position;
        }else if (cameraLibre){
            //glfwSetCursorPosCallback(window, NULL); // On met en pause le callback
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Pour remettre la souris si on l'avait enlevé
        }else if (cameraPerso){
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Pour masquer la souris sur la fenêtre
            camera_position = personnage->getRepresentant()->getPoint() + glm::vec3(0.5f,2.f,4.f);
            camera_target = personnage->getRepresentant()->getPoint() + glm::vec3(0.5f,0.0f,-3.0f) - camera_position;
        }
        */

        glm::mat4 View = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        glUniformMatrix4fv(ModelMatrix,1,GL_FALSE,&Model[0][0]);
        glUniformMatrix4fv(ViewMatrix,1,GL_FALSE,&View[0][0]);
        glUniformMatrix4fv(ProjectionMatrix,1,GL_FALSE,&Projection[0][0]);

        /*
        for (int i = 0 ; i < listeVoxel.size() ; i++){
            listeVoxel[i]->drawVoxel();
        }
        */
        //vox->drawVoxel();
        //chunky->drawChunk();

        for (int i = 0 ; i < listeChunks.size() ; i++){
            listeChunks[i]->drawChunk();
        }
        sky->drawSkybox(programID);

        //personnage->getRepresentant()->drawVoxel(programID);
        
        /*
        // Temporaire (ça simule la gravité)
        if(personnage->getRepresentant()->getPoint()[1] > 1.0f){
            personnage->move(glm::vec3(0.f,personnage->getJumpSpeed(),0.f));
            personnage->loadPerso();
            personnage->updateJumpSpeed(-0.02);  
        }else if (personnage->getRepresentant()->getPoint()[1] < 1.0f){
            personnage->move(glm::vec3(0.f,1.0f - personnage->getRepresentant()->getPoint()[1],0.f)); // Le personnage retourne en 1.0
        }
        */

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        float fps = ImGui::GetIO().Framerate;

        ImGui::NewFrame();
        ImGui::Begin("Panneau de contrôle");
        ImGui::Text("FPS : %.2f", fps);

        ImGui::Spacing();

        ImGui::SliderInt("Vitesse caméra", &speedCam, 5, 50);

        ImGui::Spacing();

        ImGui::Checkbox("Caméra orbitale", &cameraOrbitale);

        ImGui::Spacing();
        
        ImGui::Checkbox("Caméra libre", &cameraLibre);

        ImGui::Spacing();
        
        ImGui::Checkbox("Caméra personnage", &cameraPerso);

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
            buildPlanChunks();
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Largeur", &planeLength, 1, 32)){
            buildPlanChunks();
        }

        ImGui::Spacing();

        if (ImGui::SliderInt("Hauteur", &planeHeight, 1, 8)){
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