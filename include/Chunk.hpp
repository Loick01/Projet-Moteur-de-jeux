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
        std::vector<int> objectIDs;
        GLuint vertexbuffer;
        GLuint elementbuffer;
        GLuint shaderstoragebuffer;
    public:
        Chunk(glm::vec3 position, int typeChunk, unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk);
        void buildFullChunk();
        void buildFlatChunk();
        void buildSinusChunk();
        void buildProceduralChunk(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk);
        void loadChunk();
        void drawChunk();
        std::vector<Voxel*> getListeVoxels();
        void setListeVoxels(std::vector<Voxel*> newListeVoxels);
        glm::vec3 getPosition();
};