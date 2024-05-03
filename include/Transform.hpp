#pragma once


#include <Headers.hpp>

class Transform {
private:
    glm::mat3 matrix;
    glm::vec3 translation;

public:
    // Constructeur par défaut
    Transform();

    Transform(glm::vec3 t, glm::mat3 m);

    Transform(glm::mat3 m);

    Transform(glm::vec3 t);

    Transform(const glm::mat4& mat4);

    glm::mat3 getMatrix() const;

    glm::vec3 getTranslation() const;

    glm::mat4 getMatrix4();

};

