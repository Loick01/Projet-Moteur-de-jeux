#include <Personnage.hpp>

Personnage::Personnage(glm::vec3 position){
    this->currentJumpSpeed = 0.0;
    this->deplacementPerso = glm::vec3(0.0f,0.0f,0.0f);
    this->representant = new Voxel(position);

    for (int i = 0 ; i < 6 ; i++){
        this->representant->getFacesVoxel()[i]->faceId = 10; // Id réservé pour le voxel représentant le personnage
    }
}

Voxel* Personnage::getRepresentant(){
    return this->representant;
}

void Personnage::loadPerso(){
    this->representant->loadVoxel();
}

void Personnage::move(glm::vec3 motion){
    for (int i = 0 ; i < 6 ; i++){
        for (int n = 0 ; n < this->representant->getFacesVoxel()[i]->vertices.size() ; n++){
            this->representant->getFacesVoxel()[i]->vertices[n] += motion;
        }
    }
    this->representant->updatePoint(motion);
}

void Personnage::updateJumpSpeed(float js){
    this->currentJumpSpeed += js;
}

void Personnage::setJumpSpeed(float js){
    this->currentJumpSpeed = js;
}

float Personnage::getJumpSpeed(){
    return this->currentJumpSpeed;
}