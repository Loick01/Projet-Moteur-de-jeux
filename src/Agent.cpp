#include <Agent.hpp>

Agent::Agent(){
    this->mouvement.angleForLeg = 0.0f;
    this->angleOfView = 0; // Ca c'est pour le zombie, pour le cochon se sera sûrement une autre valeur (il faudra mettre un paramètre à ce constructeur pour cette valeur)
    this->isMoving = false;
    printf("creation agent \n");
}

Agent::~Agent(){
    std::cout << "Destructeur de Agent\n";
}

bool Agent::getIsMoving(){
    return this->isMoving;
}

void Agent::createMouvement(glm::vec3 d){
    this->mouvement.timeMotion = 1.0f + rand()%4;
    this->mouvement.direction = d;
    this->mouvement.direction=glm::normalize(this->mouvement.direction);

    this->mouvement.angleViewToReach = atan2(this->mouvement.direction[0],this->mouvement.direction[2]);
    

    printf("angleinitial = %f\n",this->angleOfView);
    printf("angletoreach= %f\n",this->mouvement.angleViewToReach);
    
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