#include <Player.hpp>

Player::Player(glm::vec3 position){
    this->stamina = 100.0f;
    this->life = 100.0f;
    this->hitbox = new Hitbox(position, 21.0f, 7.5f);
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
    // std::cout << "Vous prenez des dégâts : " << damage << "\n";
    this->life -= damage;
}

float Player::getLife(){
    return this->life;
}

Hitbox* Player::getHitbox(){
    return this->hitbox;
}