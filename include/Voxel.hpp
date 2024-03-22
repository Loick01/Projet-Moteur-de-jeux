#pragma once

#include <Headers.hpp>

struct Face {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indicesTriangles;
    std::vector<glm::vec2> uv;
    int faceId;

    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint uvbuffer;
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
};