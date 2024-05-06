#pragma once

#include <Headers.hpp>

class Player;

class Hitbox{
    private :
        glm::vec3 bottomPoint; // Sert pour la détection des collisions
        bool canJump;
        float gravity;
        float forceJump;
        float forceJumpInitial;
    public :
        Hitbox(glm::vec3 position, float gravity, float forceJumpInitial);
        ~Hitbox();
        glm::vec3 getBottomPoint();
        void move(glm::vec3 motion);
        bool getCanJump();
        void setCanJump(bool canJump);
        void checkJump(bool *hasUpdate, float deltaTime);
        void resetJumpForce();
        // Les 2 fonctions ci-dessous, il faudrait essayer de les condenser en une seule
        bool canGoLeftOrRight(float directionCheck, glm::vec3 bottomPlayer, glm::vec3 camera_target, glm::vec3 camera_up, int planeWidth, int planeLength, std::vector<Chunk*> listeChunks, glm::vec3 *cross_point);
        bool canGoFrontOrBack(float directionCheck, glm::vec3 bottomPlayer, glm::vec3 camera_target, int planeWidth, int planeLength, std::vector<Chunk*> listeChunks);
        void checkTopAndBottomCollision(bool hasUpdate, int planeWidth, int planeLength, float deltaTime, std::vector<Chunk*> listeChunks, Hud *hud, Player *player);
};