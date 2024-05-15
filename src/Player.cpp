#include <Player.hpp>

Player::Player(glm::vec3 position, float playerHeight, float playerWidth, float playerSpeed, float coeffAcceleration){
    this->stamina = 100.0f;
    this->life = 100.0f;
    this->hitbox = new Hitbox(position, playerHeight, playerWidth, 21.0f, 7.5f);
    this->playerSpeed = playerSpeed;
    this->coeffAcceleration = coeffAcceleration;
}

Player::~Player(){
    std::cout << "Destructeur de Player\n";
    delete this->hitbox;
}

float Player::getStamina(){
    return this->stamina;
}

void Player::addStamina(float add){
    this->stamina += add;
}

void Player::takeDamage(float damage){
    this->life -= damage;
}

float Player::getLife(){
    return this->life;
}

Hitbox* Player::getHitbox(){
    return this->hitbox;
}

void Player::setHitbox(Hitbox *hitbox){
    this->hitbox=hitbox;
}

float Player::getPlayerSpeed(){
    return this->playerSpeed;
}

float* Player::getRefToSpeed(){
    return &(this->playerSpeed);
}

float Player::getCoeffAcceleration(){
    return this->coeffAcceleration;
}

void Player::applyAcceleration(bool b){ // b = true si le joueur se met à sprinter, false s'il arrête
    this->playerSpeed *= (b?this->coeffAcceleration:1/this->coeffAcceleration);
}