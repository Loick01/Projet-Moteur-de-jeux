#pragma once

#include <Headers.hpp>

class Voxel {
    private:
        std::vector<glm::vec3> vertices;
        std::vector<unsigned short> indices;

        GLuint vertexbuffer;
        GLuint elementbuffer;
        
    public:
        Voxel();
        
        //void buildVoxel();
        void loadVoxel();
        void drawVoxel();
        /*
        std::vector<Face*> getFacesVoxel();
        glm::vec3 getPoint();
        void updatePoint(glm::vec3 motion);
        */
};