#include <Voxel.hpp>

Voxel::Voxel(glm::vec3 position, int objectID){
    this->backBottomLeftCorner = position;
    this->buildVoxel();
    this->objectID = objectID;
    this->isVisible = false;
}

Voxel::~Voxel(){
    //std::cout << "Destructeur de Voxel\n";
}

glm::vec3 Voxel::getBackBottomLeftCorner(){
    return this->backBottomLeftCorner;
}

// Voxel de taille 1.0
void Voxel::buildVoxel(){
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
    }
}

std::vector<glm::vec3> Voxel::getVertices(){
    return this->vertices;
}

void Voxel::setVisible(bool isVisible){
    this->isVisible = isVisible;
}

bool Voxel::getVisible(){
    return this->isVisible;
}

void Voxel::setId(int new_id){
    this->objectID = new_id;
}

int Voxel::getID(){
    return this->objectID;
}

void Voxel::setIdInChunk(int idInChunk){
    this->idInChunk = idInChunk;
}

int Voxel::getIdInChunk(){
    return this->idInChunk;
}