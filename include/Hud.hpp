#pragma once

#include <Headers.hpp>

struct PlaneHud{
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
    GLuint vertexbuffer;
    GLuint elementbuffer;
};

class Hud{
    private:
        PlaneHud hotbar; // Pour l'instant cette classe est faite uniquement pour la hotbar, ensuite ce sera tout l'hud
    public:
        Hud(glm::vec2 bottomLeft,float widthPlane, float heightPlane);
        void loadHud();
        void drawHud();
};