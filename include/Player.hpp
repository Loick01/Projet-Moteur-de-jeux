#pragma once

#include <Headers.hpp>

class Hitbox; 

class Chunk;
class Hud;

class Player {
    private:
        glm::vec3 bottomPoint; // Sert pour la détection de collision vers le bas
        bool canJump;
        float stamina;
        float life;
        Hitbox *hitbox;
        float gravity;
        float forceJump;
        float forceJumpInitial;
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
        void checkJump(bool *hasUpdate, float deltaTime);
        void resetJumpForce();
        // Les 2 fonctions ci-dessous, il faudrait essayer de les condenser en une seule
        bool canGoLeftOrRight(float directionCheck, glm::vec3 bottomPlayer, glm::vec3 camera_target, glm::vec3 camera_up, int planeWidth, int planeLength, std::vector<Chunk*> listeChunks, glm::vec3 *cross_point);
        bool canGoFrontOrBack(float directionCheck, glm::vec3 bottomPlayer, glm::vec3 camera_target, int planeWidth, int planeLength, std::vector<Chunk*> listeChunks);
        void checkTopAndBottomCollision(bool hasUpdate, int planeWidth, int planeLength, float deltaTime, std::vector<Chunk*> listeChunks, Hud *hud);
};