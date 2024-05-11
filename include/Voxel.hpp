#pragma once

#include <Headers.hpp>

class Voxel {
    private:
        std::vector<glm::vec3> vertices;
        int objectID;
        bool isVisible;
        int idInChunk;
    public:
        glm::vec3 backBottomLeftCorner;
        Voxel(glm::vec3 position, int objectID);
        ~Voxel();
        glm::vec3 getBackBottomLeftCorner();
        void buildVoxel();
        std::vector<glm::vec3> getVertices();
        void setVisible(bool isVisible);
        bool getVisible();
        void setId(int new_id);
        int getID();
        void setIdInChunk(int idInChunk);
        int getIdInChunk();
};