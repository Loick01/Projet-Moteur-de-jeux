#pragma once

#include <Headers.hpp>

struct Face {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indicesTriangles;
    int faceId;

    GLuint vertexbuffer;
    GLuint elementbuffer;
};

class Voxel {
    private:
        glm::vec3 backBottomLeftCorner;
        std::vector<Face*> facesVoxel; // Vecteur de 6 faces
    public:
        Voxel(glm::vec3 position);
        void buildVoxel();
        void loadVoxel();
        void drawVoxel(GLuint programID);
        std::vector<Face*> getFacesVoxel();
        glm::vec3 getPoint();
        void updatePoint(glm::vec3 motion);
};