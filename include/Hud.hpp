#pragma once

#include <Headers.hpp>

struct PlaneHud{
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
};

class Hud{
    private:
        std::vector<PlaneHud> elements; // Pour l'instant juste la hotbar
        GLuint vertexbuffer;
        GLuint elementbuffer;
        int nbIndices;
    public:
        Hud(glm::vec2 bottomLeft,float widthPlane, float heightPlane/*, float widthSelect, float heightSelect*/);
        void loadHud();
        void drawHud();
};