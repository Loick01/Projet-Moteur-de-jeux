#pragma once

#include <Headers.hpp>

#include <fstream>

#define NB_BLOCK 34
#define GRASS_BLOCK 13
#define DIRT_BLOCK 32
#define DIAMOND_ORE 19
#define IRON_ORE 21
#define STONE_BLOCK 0

struct BlockStructure{
    int infoBlock[4];
};

struct Structure{
    std::vector<BlockStructure> blocks;
};

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
        static std::vector<Structure> structures;
    public:
        Chunk(glm::vec3 position, int typeChunk, unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk, int seed);
        ~Chunk();
        void buildFullChunk();
        void buildFlatChunk();
        void buildSinusChunk();
        void buildProceduralChunk(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk, int seed);
        void loadChunk();
        void drawChunk();
        std::vector<Voxel*> getListeVoxels();
        void setListeVoxels(std::vector<Voxel*> newListeVoxels);
        glm::vec3 getPosition();

        // Pour la génération des structures
        static Structure readFile(std::ifstream &file);
        static void setListeStructures(std::vector<Structure> liste);
        void buildStructure(int i, int j, int k);
};