#include <Voxel.hpp>

Voxel::Voxel(glm::vec3 position, int numBlock){
    this->backBottomLeftCorner = position;
    this->buildVoxel(numBlock);
    this->isVisible = false;
}

void Voxel::buildVoxel(int numBlock){
    //std::cout << "Construction des faces du voxel\n";
    // Voxel de taille 1.0

    for (int i = 0 ; i < 6 ; i++){
        for (int h = 0; h < 2 ; h++) {
            for (int w = 0; w < 2; w++) {
                float x, y, z;
                int n = (i%2 == 0 ? 1 : -1);

                if (i < 2){ // Faces bottom et top
                    x = this->backBottomLeftCorner[0] + (float)w;
                    y = this->backBottomLeftCorner[1] + i;
                    z = this->backBottomLeftCorner[2] + i + (float)h * n; 
                }else if (i == 2){ // Faces back
                    x = this->backBottomLeftCorner[0] + (1-w);
                    y = this->backBottomLeftCorner[1] + (float)h; 
                    z = this->backBottomLeftCorner[2];
                }else if (i == 3){ // Faces front
                    x = this->backBottomLeftCorner[0] + (float)w;
                    y = this->backBottomLeftCorner[1] + (float)h; 
                    z = this->backBottomLeftCorner[2] + 1.0f;
                }else if (i == 4){ // Face left
                    x = this->backBottomLeftCorner[0] + (i-4);
                    y = this->backBottomLeftCorner[1] + (float)h;
                    z = this->backBottomLeftCorner[2] + (float)w; 
                }else if (i == 5){ // Face right
                    x = this->backBottomLeftCorner[0] + (i-4);
                    y = this->backBottomLeftCorner[1] + (float)h;
                    z = this->backBottomLeftCorner[2] + (1-w); 
                }
                this->vertices.push_back(glm::vec3(x,y,z));
            }
        }

        unsigned int decalage = 24*numBlock + i*4; // 24 sommets par voxel
        this->indices.push_back(decalage + 2);
        this->indices.push_back(decalage + 0);
        this->indices.push_back(decalage + 3);
        this->indices.push_back(decalage + 3);
        this->indices.push_back(decalage + 0);
        this->indices.push_back(decalage + 1);
    }
}

std::vector<glm::vec3> Voxel::getVertices(){
    return this->vertices;
}
std::vector<unsigned int> Voxel::getIndices(){
    return this->indices;
}

/*
glm::vec3 Voxel::getPoint(){
    return this->backBottomLeftCorner;
}

void Voxel::updatePoint(glm::vec3 motion){
    this->backBottomLeftCorner += motion;
}
*/

void Voxel::setVisible(){
    this->isVisible = true;
}

bool Voxel::getVisible(){
    return this->isVisible;
}