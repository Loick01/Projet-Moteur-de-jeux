#pragma once

#include <Headers.hpp>

#define RANGE 4

class MapGenerator;
class Chunk;

struct LocalisationBlock {
    int indiceVoxel, indiceChunk, numLongueur, numProfondeur, numHauteur, idInChunk;
};

class Entity;

class TerrainControler{
    private :
        // Ces 3 tailles sont en nombre de chunk
        int planeWidth; // De 1 à 32
        int planeLength; // De 1 à 32
        int planeHeight; // Pour simplifier on bloque cette valeur à 1
        std::vector<Chunk*> listeChunks;
        int typeChunk; // Chunk plein (0), Chunk sinus (1), Chunk plat (2), Chunk procédural (3)
        int seedTerrain; 
        int octave;
        MapGenerator *mg;
        float accumulateurDestructionBlock;
        bool mouseLeftClickHold;
        int previousIdInChunk; 

    public :
        TerrainControler(int planeWidth, int planeLength, int planeHeight, int typeChunk, int seedTerrain, int octave, std::vector<std::vector<std::string>> nomStructure);
        TerrainControler(); // Ce deuxième constructeur ne sera appelé que pour créer le terrain utilisé par le mode éditeur
        ~TerrainControler();
        std::vector<Chunk*> getListeChunks();
        void buildPlanChunks(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap);
        void buildEditorChunk();
        int getPlaneWidth();
        int* getRefToPlaneWidth();
        int getPlaneLength();
        int* getRefToPlaneLength();
        int getPlaneHeight();
        int* getRefToSeedTerrain();
        int* getRefToOctave();
        LocalisationBlock tryBreakBlock(glm::vec3 camera_target, glm::vec3 camera_position);
        void breakBlock(LocalisationBlock lb);
        bool tryCreateBlock(glm::vec3 camera_target, glm::vec3 camera_position, int typeBlock);
        void drawTerrain();
        void saveStructure(std::string filePath);
        bool checkHoldLeftClick(glm::vec3 camera_position, glm::vec3 camera_target, float deltaTime, bool modeJeu, GLuint programID);
        void setMouseLeftClickHold(bool mouseLeftClickHold);
        bool getMouseLeftClickHold();
        int getPreviousIdInChunk();
        void setPreviousIdInChunk(int previousIdInChunk);
        void setAccumulation(float accumulation);
        MapGenerator* getMapGenerator();
};