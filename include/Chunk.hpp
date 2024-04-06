#pragma once

#include <Headers.hpp>

class Voxel; // Déclaration avancée (je ne comprends pas pourquoi on est obligé de faire ça, avant ce n'était pas nécéssaire)

// Chunk de taille 32x32x32
class Chunk{
    private:
        glm::vec3 position;
        std::vector<Voxel*> listeVoxels;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> vertices;
        GLuint vertexbuffer;
        GLuint elementbuffer;
        GLuint shaderstoragebuffer;
    public:
        Chunk(glm::vec3 position, int typeChunk/*, GLubyte *texels, GLint widthTexture, GLint heightTexture*/);
        void buildFullChunk(/*GLubyte *texels, GLint widthTexture, GLint heightTexture*/);
        void buildSinusChunk();
        void loadChunk();
        void drawChunk();
        std::vector<Voxel*> getListeVoxels();
        void setListeVoxels(std::vector<Voxel*> newListeVoxels);
};