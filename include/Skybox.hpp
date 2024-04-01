#pragma once

#include <Headers.hpp>

class Skybox {
    private:
        float skyboxVertices[108];
        std::string pathTextures[6];
        GLuint programID_skybox;
        GLuint textureID;
        unsigned int skyboxVAO;

    public:
        Skybox();
        void loadCubemap();
        void bindCubemap(GLenum TextureUnit, int unit);
        void useShader();
        GLuint getProgramID();
        unsigned int getVAO();
};