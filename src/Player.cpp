#include <Player.hpp>

Player::Player(glm::vec3 position){
    //this->hitbox = new Voxel(position,0); // Le deuxième paramètre doit rester à 0 (sert normalement à la génération des chunks, mais ici on est pas concerné)
    this->bottomPoint = glm::vec3(position + glm::vec3(0.5f,0.0f,0.5f));
    this->currentJumpSpeed = 0.0f;
    this->canJump = false;
}

/*
void Player::loadPlayer(){
    glGenBuffers(1, &(this->vertexbufferHitbox));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbufferHitbox);
    glBufferData(GL_ARRAY_BUFFER, this->hitbox->getVertices().size() * sizeof(glm::vec3), &(this->hitbox->getVertices()[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbufferHitbox));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbufferHitbox);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->hitbox->getIndices().size() * sizeof(unsigned int), &(this->hitbox->getIndices()[0]) , GL_STATIC_DRAW);
}

void Player::drawPlayer(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbufferHitbox);
    glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbufferHitbox);

    // Draw the triangles !
    glDrawElements(
                    GL_TRIANGLES,      // mode
                    this->hitbox->getIndices().size(), // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
}
*/

void Player::move(glm::vec3 motion){
    this->bottomPoint += motion;
    //this->hitbox->updateVertices(motion);
}

glm::vec3 Player::getBottomPoint(){
    return this->bottomPoint;
}

float Player::getJumpSpeed(){
    return this->currentJumpSpeed;
}

void Player::addToSpeed(float s){
    this->currentJumpSpeed += s;
}

bool Player::getCanJump(){
    return this->canJump;
}

void Player::couldJump(bool t){
    this->canJump = t;
}

void Player::resetJumpSpeed(){
    this->currentJumpSpeed = 0.0f;
}