#include <Player.hpp>

Player::Player(glm::vec3 position){
    this->bottomPoint = glm::vec3(position + glm::vec3(0.5f,0.0f,0.5f));
    this->canJump = false;
    this->stamina = 100.0f;
    this->life = 100.0f;
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

float Player::getStamina(){
    return this->stamina;
}

void Player::addStamina(float add){
    this->stamina += add;
}

void Player::takeDamage(float damage){
    // std::cout << "Vous prenez des dégâts : " << damage << "\n";
    this->life -= damage;
}

float Player::getLife(){
    return this->life;
}