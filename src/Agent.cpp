#include <Agent.hpp>

Agent::Agent(float angleOfView){
    this->mouvement.angleForLeg = 0.0f;
    this->angleOfView = angleOfView;
    this->isMoving = false;
    this->isAttacking = false;
    this->accumulateurAnimation=0.0f;
}

Agent::~Agent(){
    std::cout << "Destructeur de Agent\n";
}

bool Agent::getIsMoving(){
    return this->isMoving;
}

void Agent::createMouvement(glm::vec3 d){
    this->mouvement.timeMotion = 1.0f + rand()%2;
    this->mouvement.direction = d;
    this->mouvement.direction=glm::normalize(this->mouvement.direction);
    this->mouvement.angleViewToReach = atan2(this->mouvement.direction[0],this->mouvement.direction[2]);
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
    // printf("angle = %f\n",this->angleOfView);
    
    if(this->angleOfView>M_PI)this->angleOfView=-M_PI;
    if(this->angleOfView<-M_PI)this->angleOfView=M_PI;

}

float Agent::getAngleOfView(){
    return this->angleOfView;
}

float Agent::getAngleToReach(){
    return this->mouvement.angleViewToReach;
}


void Agent::setDirection(glm::vec3 d){
    this->mouvement.direction=d;
}

void Agent::resetAccumulateur(){
    this->accumulateurAnimation = 0.0f;
}

float Agent::getAccumulateur(){
    return this->accumulateurAnimation;
}

void Agent::setAccumulateur(float accumulateurAnimation){
    this->accumulateurAnimation=accumulateurAnimation;
}

bool Agent::getIsAttacking(){
    return this->isAttacking;
}

void Agent::setIsAttacking(bool isAttacking){
    this->isAttacking=isAttacking;
}