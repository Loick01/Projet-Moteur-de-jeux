#pragma once

#include <Headers.hpp>

class Voxel; // Déclaration avancée

class Player {
    private:
        /*
        Voxel *representant; // Pour l'instant, le personnage est représenté par un bloc
        */
        Voxel *hitbox; // Pour l'instant la hitbox est représenté juste avec un voxel
        GLuint vertexbufferHitbox;
        GLuint elementbufferHitbox;
        glm::vec3 bottomPoint; // Sert pour la détection de collision vers le bas
        float currentJumpSpeed;
        bool canJump; // Le joueur ne peut sauter que s'il est au sol
    public:
        Player(glm::vec3 position);
        void loadPlayer();
        void drawPlayer();
        void move(glm::vec3 motion);
        glm::vec3 getBottomPoint();
        void setJumpSpeed(float initialeSpeed);
        bool getCanJump();
        /*
        Voxel* getRepresentant();
        void updateJumpSpeed(float js);
        void setJumpSpeed(float js);
        float getJumpSpeed();
        */
};