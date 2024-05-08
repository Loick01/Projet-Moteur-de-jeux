#include <Transform.hpp>

Transform::Transform() {
    this->m = glm::mat3(1.0f); // Neutre
    this->t = glm::vec3(0.0f);  // Neutre
}

Transform::Transform(glm::vec3 t, glm::mat3 m) {
    this->m = m;
    this->t = t;
}

Transform::Transform(glm::mat3 m) {
    this->m = m;
    this->t = glm::vec3(0.0f); // Neutre
}

Transform::Transform(glm::vec3 t) {
    this->m = glm::mat3(1.0f); // Neutre
    this->t = t;
}

Transform::Transform(glm::mat4 mat4) {
    this->t = glm::vec3(mat4[3]); // La translation est dans la quatrième colonne de la mat4
    glm::mat3 mat3; // On veut extraire la mat3 de la mat4
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            mat3[i][j] = mat4[i][j];
        }
    }
    this->m = mat3;
}

glm::mat3 Transform::getRotationScalingMatrix() {
    return this->m;
}

glm::vec3 Transform::getTranslationVector() {
    return this->t;
}

glm::mat4 Transform::getTransfoMat4(){
    glm::mat4 result = glm::mat4(this->m);
    result[3] = glm::vec4(this->t[0],this->t[1],this->t[2],1);
    return result;
}

void Transform::addVelocity(glm::vec3 t){
    this->t += t;
}


