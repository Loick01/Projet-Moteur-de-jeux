#pragma once

#include <Headers.hpp>

class Skybox {
    private:
        float skyboxVertices[24];
        unsigned int indices[36];
        std::string pathTextures[6];
        GLuint vertexbuffer;
        GLuint elementbuffer;
        GLuint programID_skybox;
        unsigned int textureID;

    public:
        Skybox();
        void loadCubemap();
        void loadSkybox();
        void drawSkybox();
        GLuint getShaderID();
};