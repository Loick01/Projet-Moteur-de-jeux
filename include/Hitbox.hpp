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
        // La fonction ci-dessous vérifie si le déplacement latéral demandé est possible ou non
        bool getLateralMovePossible(bool axisToCheck, float directionCheck, glm::vec3 bottomPlayer, glm::vec3 camera_target, glm::vec3 camera_up, int planeWidth, int planeLength, std::vector<Chunk*> listeChunks, glm::vec3 *cross_point);
        void checkTopAndBottomCollision(bool hasUpdate, int planeWidth, int planeLength, float deltaTime, std::vector<Chunk*> listeChunks, Hud *hud, Player *player);
};