#pragma once

#include <Headers.hpp>

struct PlaneHud{
    // Chaque plan du hud est définit par 4 sommets, et 6 indices qui forment 2 triangles 
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
    bool visible;
};

class Hud{
    private:
        std::vector<PlaneHud> elements;
        GLuint vertexbuffer;
        GLuint elementbuffer;
        int nbIndices;
        PlaneHud hotbar;
        PlaneHud select;
        PlaneHud cursor;
        PlaneHud inventory;
    public:
        Hud(int screen_width, int screen_height);
        PlaneHud createPlaneHud(glm::vec2 refPoint, float widthPlane, float heightPlane, int decalage);
        void loadHud();
        void drawHud();
};