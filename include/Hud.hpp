#pragma once

#include <Headers.hpp>

struct PlaneHud{
    // Chaque plan du hud est définit par 4 sommets, et 6 indices qui forment 2 triangles 
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;
};

class Hud{
    private:
        int screen_width;
        float hotbarWidth;
        std::vector<PlaneHud> elements;
        GLuint vertexbuffer;
        GLuint elementbuffer;
        int nbIndices;
        PlaneHud hotbar;
        PlaneHud select;
        PlaneHud cursor;
        PlaneHud lifeLine;
        PlaneHud staminaLine;
        PlaneHud lifeBar;
        PlaneHud staminaBar;
    public:
        Hud(int screen_width, int screen_height);
        ~Hud();
        PlaneHud createPlaneHud(glm::vec2 refPoint, float widthPlane, float heightPlane, int decalage);
        void loadHud();
        void drawHud();
        void updateLife(float new_life);
        void updateStamina(float new_stamina);
};