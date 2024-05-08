#pragma once


#include <Headers.hpp>

class Transform {
private:
    glm::mat3 m; // Rotation + Scaling
    glm::vec3 t; // Translation
public:
    Transform();
    Transform(glm::vec3 t, glm::mat3 m);
    Transform(glm::mat3 m);
    Transform(glm::vec3 t);
    Transform(glm::mat4 mat);

    glm::mat3 getRotationScalingMatrix();
    glm::vec3 getTranslationVector();
    glm::mat4 getTransfoMat4();

    void addVelocity(glm::vec3 t);
};
