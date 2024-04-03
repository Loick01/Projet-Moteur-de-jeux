#pragma once

#include <Headers.hpp>

class Voxel {
    private:
        glm::vec3 backBottomLeftCorner;
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        //int faceId;
        bool isVisible;
    public:
        Voxel(glm::vec3 position, int numBlock);
        void buildVoxel(int numBlock);
        glm::vec3 getPoint();
        void updateVertices(glm::vec3 motion);
        std::vector<glm::vec3> getVertices();
        std::vector<unsigned int> getIndices();
        void setVisible(bool isVisible);
        bool getVisible();
        void shiftIndice();
};