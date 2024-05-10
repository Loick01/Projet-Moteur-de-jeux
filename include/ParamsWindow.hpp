#pragma once

#include <Headers.hpp>

// Temporaire (tant que ces variables n'ont pas été mise dans des classes)
#include "variables.h"

class ParamsWindow{
    private:
        int style; // 0 pour Dark, 1 pour Light, sinon Classic
        bool renduFilaire;
        TerrainControler *terrainControler;
    public:
        ParamsWindow(GLFWwindow* window, int style, TerrainControler *terrainControler);
        ~ParamsWindow();
        void useStyle();
        void init(GLFWwindow* window);
        void draw();
};