#pragma once

#include <Headers.hpp>

class Hitbox; 

class Chunk;
class Hud;

class Player {
    private:
        float stamina;
        float life;
        Hitbox *hitbox;
    public:
        Player(glm::vec3 position);
        ~Player();
        float getStamina();
        void addStamina(float add);
        void takeDamage(float damage);
        float getLife();
        Hitbox* getHitbox();
};