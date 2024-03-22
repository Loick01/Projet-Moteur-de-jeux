#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    private : 
        int height, width;
        const char* title;
        unsigned int contextMajor, contextMinor;
        GLFWwindow* window;

    public :
        Window(unsigned int contextMajor, unsigned int contextMinor, int width, int height, const char *title);
        ~Window();
        void setup_GLFW();

        int get_height();
        int get_width();
        const char* get_title();

        unsigned int get_contextMajor();
        unsigned int get_contextMinor();
        GLFWwindow* get_window();  
};