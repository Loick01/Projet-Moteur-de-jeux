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
        float playerSpeed;
        float coeffAcceleration; // Valeur par laquelle la vitesse est multiplié lorsque le joueur sprinte
    public:
        Player(glm::vec3 position, float playerHeight, float playerWidth, float playerSpeed, float coeffAcceleration);
        ~Player();
        float getPlayerSpeed();
        float* getRefToSpeed();
        float getCoeffAcceleration();
        void applyAcceleration(bool b);
        float getStamina();
        void addStamina(float add);
        void takeDamage(float damage);
        float getLife();
        Hitbox* getHitbox();
        void setHitbox(Hitbox *hitbox);
};