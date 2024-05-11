#pragma once

#include <Headers.hpp>

#define RANGE 4

class MapGenerator;
class Chunk;

struct LocalisationBlock {
    int indiceVoxel, indiceChunk, numLongueur, numProfondeur, numHauteur, idInChunk;
};


class TerrainControler{
    private :
        // Ces 3 tailles sont en nombre de chunk
        int planeWidth; // De 1 à 32
        int planeLength; // De 1 à 32
        int planeHeight; // Pour simplifier on bloque cette valeur à 1
        std::vector<Chunk*> listeChunks;
        int typeChunk; // Chunk plein (0), Chunk sinus (1), Chunk plat (2), Chunk procédural (3)
        int seedTerrain; // 1000
        int octave; // 4
        MapGenerator *mg;

    public :
        TerrainControler(int planeWidth, int planeLength, int planeHeight, int typeChunk, int seedTerrain, int octave, std::vector<std::string> nomStructure);
        TerrainControler(); // Ce deuxième constructeur ne sera appelé que pour créer le terrain utilisé par le mode éditeur
        ~TerrainControler();
        std::vector<Chunk*> getListeChunks();
        void buildPlanChunks(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap);
        void buildEditorChunk();
        int getPlaneWidth();
        int getPlaneLength();
        int getPlaneHeight();
        LocalisationBlock tryBreakBlock(glm::vec3 camera_target, glm::vec3 camera_position);
        void breakBlock(LocalisationBlock lb);
        void tryCreateBlock(glm::vec3 camera_target, glm::vec3 camera_position, int typeBlock);
        void drawTerrain();
        void saveStructure(char* nameStructure);
};