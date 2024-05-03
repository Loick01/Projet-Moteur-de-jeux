#pragma once

#include <Headers.hpp>

class Skybox {
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        GLuint vertexbuffer;
        GLuint elementbuffer;

        std::string pathTextures[6];
        GLuint textureID;

        GLuint programID_Skybox;

    public:
        Skybox();
        ~Skybox();
        void createSkybox();
        void drawSkybox(glm::mat4 Model, glm::mat4 Projection, glm::mat4 View);

        void loadCubemap();
        void bindCubemap(GLenum TextureUnit, int unit);
};