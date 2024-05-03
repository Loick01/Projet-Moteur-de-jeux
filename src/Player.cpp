#include <Player.hpp>

Player::Player(glm::vec3 position){
    this->bottomPoint = glm::vec3(position + glm::vec3(0.5f,0.0f,0.5f));
    this->canJump = false;
}

Player::~Player(){ // Si on laisse un destructeur vide, valgrind aime pas ça
    std::cout << "Destructeur de Player\n";
}

void Player::move(glm::vec3 motion){
    this->bottomPoint += motion;
}

glm::vec3 Player::getBottomPoint(){
    return this->bottomPoint;
}

bool Player::getCanJump(){
    return this->canJump;
}

void Player::setCanJump(bool canJump){
    this->canJump = canJump;
}