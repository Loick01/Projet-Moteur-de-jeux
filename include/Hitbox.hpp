#pragma once

#include <Headers.hpp>

class Player;
class TerrainControler;

// Cette classe sera utilisée pour le joueur, mais aussi pour les entités (donc on fait en sorte qu'elle soit la plus générale possible) 
class Hitbox{
    private :
        glm::vec3 bottomPoint;
        float heightHitbox;
        float widthHitbox;
        float lengthHitbox;
        bool canJump;
        float gravity;
        float forceJump;
        float forceJumpInitial;
        bool canTakeDamage; // On ne subira pas de dégâts avant le premier contact avec un bloc (pour ne pas mourir dès l'apparition)
    public :
        Hitbox(glm::vec3 position, float heightHitbox, float widthHitbox, float gravity, float forceJumpInitial);
        Hitbox(glm::vec3 position, float heightHitbox, float widthHitbox, float lengthHitbox, float gravity, float forceJumpInitial);
        ~Hitbox();
        glm::vec3 getBottomPoint();
        glm::vec3* getRefToBottomPoint();
        void move(glm::vec3 motion);
        bool getCanJump();
        void setCanJump(bool canJump);
        void checkJump(bool *hasUpdate, float deltaTime);
        void resetJumpForce(int amplitude);
        // La fonction ci-dessous vérifie si le déplacement latéral demandé est possible ou non
        bool getLateralMovePossible(bool axisToCheck, float directionCheck, glm::vec3 camera_target, glm::vec3 camera_up, TerrainControler *terrainControler, glm::vec3 *cross_point);
        float checkTopAndBottomCollision(bool hasUpdate, float deltaTime, TerrainControler *terrainControler);
        void resetCanTakeDamage();
};