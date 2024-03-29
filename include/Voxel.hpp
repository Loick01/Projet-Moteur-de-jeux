#pragma once

#include <Headers.hpp>

class Voxel {
    private:
        glm::vec3 backBottomLeftCorner;
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        //int faceId;
    public:
        Voxel(glm::vec3 position, int numBlock);
        void buildVoxel(int numBlock);
        glm::vec3 getPoint();
        void updatePoint(glm::vec3 motion);
        std::vector<glm::vec3> getVertices();
        std::vector<unsigned int> getIndices();
};