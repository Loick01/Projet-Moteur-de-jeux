#pragma once

#include <Headers.hpp>

class Skybox {
    private:
        glm::vec3 backBottomLeftCorner;
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        float size;
        GLuint vertexbuffer;
        GLuint elementbuffer;

    public:
        Skybox(float size, glm::vec3 position);
        void buildSkybox();
        void loadSkybox();
        void drawSkybox(GLint programID);
};