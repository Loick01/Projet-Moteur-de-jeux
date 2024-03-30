#pragma once

#include <Headers.hpp>

// Chunk de taille 32x32x32
class Chunk{
    private:
        glm::vec3 position;
        std::vector<Voxel*> listeVoxels;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> vertices;
        GLuint vertexbuffer;
        GLuint elementbuffer;
    public:
        Chunk(glm::vec3 position, GLubyte *texels, GLint widthTexture, GLint heightTexture);
        void buildChunk(GLubyte *texels, GLint widthTexture, GLint heightTexture);
        void loadChunk();
        void drawChunk();
};