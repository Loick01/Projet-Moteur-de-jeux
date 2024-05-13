#include <Agent.hpp>

Agent::Agent(){
    this->mouvement.angleForLeg = 0.0f;
    this->angleOfView = -M_PI/2; // Ca c'est pour le zombie, pour le cochon se sera sûrement une autre valeur (il faudra mettre un paramètre à ce constructeur pour cette valeur)
    this->isMoving = false;
}

Agent::~Agent(){
    std::cout << "Destructeur de Agent\n";
}

bool Agent::getIsMoving(){
    return this->isMoving;
}

void Agent::createMouvement(){
    this->mouvement.timeMotion = 1.0f + rand()%4;
    this->mouvement.direction[0]= -1.0f + ((rand()%21)/10.0f);
    this->mouvement.direction[2]= -1.0f + ((rand()%21)/10.0f);
    this->mouvement.direction=glm::normalize(this->mouvement.direction);
    this->mouvement.angleViewToReach = acos(this->mouvement.direction[0]);
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

void Agent::addToAngleOfView(float angleOfView){
    this->angleOfView += angleOfView;
    /*
    if(this->angleOfVue>2*M_PI)angleOfVue=0;
    if(this->angleOfVue<0)angleOfVue=2*M_PI;
    */
}

float Agent::getAngleOfView(){
    return this->angleOfView;
}

float Agent::getAngleToReach(){
    return this->mouvement.angleViewToReach;
}