#pragma once

#include <Headers.hpp>

class Voxel {
    private:
        glm::vec3 backBottomLeftCorner;
        std::vector<glm::vec3> vertices;
        std::vector<unsigned short> indices;
        //int faceId;
        GLuint vertexbuffer;
        GLuint elementbuffer;
    public:
        Voxel(glm::vec3 position);
        void buildVoxel();
        void loadVoxel();
        void drawVoxel();
        glm::vec3 getPoint();
        void updatePoint(glm::vec3 motion);
};