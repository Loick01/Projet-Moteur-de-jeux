#pragma once

#include <Headers.hpp>


struct Mouvement{
    float timeMotion;
    glm::vec3 direction;
    float angleForLeg;
    float angleViewToReach;
};

class Agent{
    private :
        bool isMoving;
        Mouvement mouvement;
        float angleOfView;
        float accumulateurAnimation;
        bool isAttacking;
    public :
        Agent(float angleOfView);
        ~Agent();
        void resetMouvement();
        void createMouvement(glm::vec3 d);
        bool getIsMoving();
        float getRemainingTime();
        void setIsMoving(bool isMoving);
        void addToAngleForLeg(float angleForLeg);
        float getAngleForLeg();
        void timePass(float timeMotion);
        glm::vec3 getDirection();
        void addToAngleOfView(float angleOfView);
        float getAngleOfView();
        float getAngleToReach();
        void setDirection(glm::vec3 d);
        void resetAccumulateur();
        float getAccumulateur();
        void setAccumulateur(float accumulateurAnimation);
        bool getIsAttacking();
        void setIsAttacking(bool isAttacking);
};