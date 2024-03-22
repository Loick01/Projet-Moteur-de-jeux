#include <Headers.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ 
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){ 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // Touche échap
        glfwSetWindowShouldClose(window,true);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280,720,"Projet Moteur de jeux",NULL,NULL);
    if (window == NULL){
        std::cout << "Erreur lors de la création de la fenêtre GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Erreur lors de l'initialisation de GLAD\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0,0,1280,720); 

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // On définit le callback à appeler lors du redimensionnement de la fenêtre
    glClearColor(0.09f,0.27f,0.22f,1.0f); 

    // Boucle de rendu
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); // Supprime et nettoie proprement les ressources GLFW allouées
    return 0;


}