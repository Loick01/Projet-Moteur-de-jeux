#include <Transform.hpp>

// Constructeur par défaut
Transform::Transform() {
    matrix = glm::mat3(1.0f); 
    translation = glm::vec3(0.0f); 
}

Transform::Transform(glm::vec3 t, glm::mat3 m) {
    matrix = m;
    translation = t;
}

Transform::Transform(glm::mat3 m) {
    matrix = m;
    translation = glm::vec3(0.0f);
}

Transform::Transform(glm::vec3 t) {
    matrix = glm::mat3(1.0f);
    translation = t;
}

Transform::Transform(const glm::mat4& mat4) {
    translation = glm::vec3(mat4[3]);
    glm::mat3 mat3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            mat3[i][j] = mat4[i][j];
        }
    }
    matrix = mat3;
}

glm::mat3 Transform::getMatrix() const {
    return matrix;
}

glm::vec3 Transform::getTranslation() const {
    return translation;
}

glm::mat4 Transform::getMatrix4(){
    glm::mat4 matrix = glm::mat4(this->matrix);
    matrix[3]= glm::vec4(this->translation[0],this->translation[1],this->translation[2],1);
    return matrix;
}

