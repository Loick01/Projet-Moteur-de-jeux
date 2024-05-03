#pragma once

#include <Headers.hpp>

class Voxel {
    private:
        std::vector<glm::vec3> vertices;
        int objectID;
        bool isVisible;
    public:
        glm::vec3 backBottomLeftCorner;
        Voxel(glm::vec3 position, int objectID);
        ~Voxel();
        void buildVoxel();
        glm::vec3 getPoint();
        //void updateVertices(glm::vec3 motion);
        std::vector<glm::vec3> getVertices();
        void setVisible(bool isVisible);
        bool getVisible();
        void setId(int new_id);
        int getID();
};