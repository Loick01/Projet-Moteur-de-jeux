#include <Agent.hpp>

Agent::Agent(){
    this->mouvement.angleForLeg = 0.0f;
    this->angleOfView = 0.0f;
    this->isMoving = false;
}

Agent::~Agent(){
    std::cout << "Destructeur de Agent\n";
}

bool Agent::getIsMoving(){
    return this->isMoving;
}

void Agent::createMouvement(){
    this->mouvement.timeMotion = 1.0f + rand()%10;
    this->mouvement.direction[0]= -1.0f + ((rand()%21)/10.0f);
    this->mouvement.direction[2]= -1.0f + ((rand()%21)/10.0f);
    this->mouvement.direction=glm::normalize(this->mouvement.direction);
    glm::vec2 v = glm::vec2(this->mouvement.direction[0],this->mouvement.direction[2]);
    glm::vec2 u = glm::vec2(cos(this->angleOfView),sin(this->angleOfView));
    this->mouvement.angleViewToReach = acos(glm::dot(u,v)/cos(this->angleOfView));
    this->isMoving=true;
}

float Agent::getRemainingTime(){
    return this->mouvement.timeMotion;
}

void Agent::setIsMoving(bool isMoving){
    this->isMoving = isMoving;
}

void Agent::addToAngleForLeg(float angleForLeg){
    this->mouvement.angleForLeg += angleForLeg;
}

float Agent::getAngleForLeg(){
    return this->mouvement.angleForLeg;
}

void Agent::timePass(float timeMotion){
    this->mouvement.timeMotion -= timeMotion;
}

glm::vec3 Agent::getDirection(){
    return this->mouvement.direction;
}