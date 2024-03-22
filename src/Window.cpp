#include <Window.hpp>

Window::Window(unsigned int contextMajor, unsigned int contextMinor, int width, int height, const char *title) {
    this->contextMajor = contextMajor;
    this->contextMinor = contextMinor;
    this->height = height;
    this->width = width;
    this->title = title;
}

void Window::setup_GLFW() {
    glfwInit();
    if (!glfwInit()) {
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->contextMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->contextMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(this->window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << "\n";
        exit(2);
    }

    glViewport(0, 0, this->width, this->height);

    glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

int Window::get_height() {
    return this->height;
}

int Window::get_width() {
    return this->width;
}

const char * Window::get_title() {
    return this->title;
}

unsigned int Window::get_contextMajor() {
    return this->contextMajor;
}

unsigned int Window::get_contextMinor() {
    return this->contextMinor;
}

GLFWwindow* Window::get_window() {
    return this->window;
}