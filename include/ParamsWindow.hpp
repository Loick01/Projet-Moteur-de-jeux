#pragma once

#include <Headers.hpp>

// Temporaire (tant que ces variables n'ont pas été mise dans des classes)
#include "variables.h"

class ParamsWindow{
    private:
        int style; // 0 pour Dark, 1 pour Light, sinon Classic
        bool renduFilaire;
        TerrainControler *terrainControler;
        bool inEditor;
        static char nameStructure[128]; // On met cet attribut en static pour que le texte saisi reste le même entre 2 frames
    public:
        ParamsWindow(GLFWwindow* window, int style, TerrainControler *terrainControler);
        ~ParamsWindow();
        void useStyle();
        void init(GLFWwindow* window);
        void draw();
        bool getInEditor();
        void attachNewTerrain(TerrainControler *terrainControler);
};