#pragma once

#include <Headers.hpp>

class Player {
    private:
        glm::vec3 bottomPoint; // Sert pour la détection de collision vers le bas
        bool canJump;
        float stamina;
        float life;
    public:
        Player(glm::vec3 position);
        ~Player();
        void move(glm::vec3 motion);
        glm::vec3 getBottomPoint();
        glm::vec3 getLeftBottomPoint();
        bool getCanJump();
        void setCanJump(bool canJump);
        float getStamina();
        void addStamina(float add);
        void takeDamage(float damage);
        float getLife();
};