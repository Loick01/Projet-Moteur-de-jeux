#include <Chunk.hpp>

#define CHUNK_SIZE 32

Chunk::Chunk(glm::vec3 position, int typeChunk/*, GLubyte *texels, GLint widthTexture, GLint heightTexture*/){
    this->position = position;
    if (typeChunk==0){
        this->buildFullChunk(/*texels, widthTexture, heightTexture*/);
    }else if (typeChunk==1){
        this->buildSinusChunk();
    }else if (typeChunk==2){
        this->buildCrazyChunk();
    }
}

void Chunk::buildFullChunk(/*GLubyte *texels, GLint widthTexture, GLint heightTexture*/){
    this->listeVoxels.clear();
    short compteur = 0; // Dénombre les voxels qui seront envoyés aux shaders, pour savoir quel décalage appliquer

    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){     
                //if (k < ((short)texels[((j*heightTexture)/31)*widthTexture + ((i*widthTexture)/31)]*32)/255){ // Pas sûr que ça fonctionne
                    Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),compteur); 
                    if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==CHUNK_SIZE-1){
                        vox->setVisible();
                        ++compteur;
                    }
                    this->listeVoxels.push_back(vox);
                //}
            }
        }
    }
}

// Avec ce modèle de génération, on voit qu'il y a un problème sur la détection de l'extérieur. Il faudra un algo pour déterminer les contours d'un chunk
void Chunk::buildSinusChunk(){
    this->listeVoxels.clear();
    short compteur = 0; // Dénombre les voxels qui seront envoyés aux shaders, pour savoir quel décalage appliquer

    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){
                float t = (float)i / CHUNK_SIZE-1;
                float s = (float)j / CHUNK_SIZE-1;
                float value = (std::sin(4 * M_PI * ((t+s)/2)) + 1.0f) * 0.5f; // Valeur entre 0 et 1
                //float value = (std::sin(2 * M_PI * t) + 1.0f) * 0.5f; // Valeur entre 0 et 1
                int heightVox = value*30 + 1; // Hauteur du cube entre 1 et 31
                if (k <= heightVox){
                    Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),compteur); 
                    if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==heightVox){
                        vox->setVisible();
                        ++compteur;
                    }
                    this->listeVoxels.push_back(vox);
                }else{
                    // Temporaire (ce ne sera sûrement pas la manière définitive de faire)
                    this->listeVoxels.push_back(nullptr); // Pour tester les collisions avec le joueur, on doit qu'il n'y a pas de voxel à cet emplacement
                }
            }
        }
    }
}

// Temporaire
void Chunk::buildCrazyChunk(){
    this->listeVoxels.clear();

    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){     
                    Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),k*CHUNK_SIZE*CHUNK_SIZE + j*CHUNK_SIZE + i); 
                    if (i*j==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==CHUNK_SIZE-1){
                        vox->setVisible();
                    }
                    this->listeVoxels.push_back(vox);
            }
        }
    }
}

void Chunk::loadChunk(){
    // Peut être faudrait il stocker les sommets et les indices directement dans la classe Chunk, au lieu de les récupérer pour chaque voxel
    for (int i = 0 ; i < this->listeVoxels.size() ; i++){
        if (listeVoxels[i] != nullptr){
            if (listeVoxels[i]->getVisible()){
                std::vector<glm::vec3> verticesVoxel = listeVoxels[i]->getVertices();
                std::vector<unsigned int> indicesVoxel = listeVoxels[i]->getIndices();
                this->vertices.insert(this->vertices.end(), verticesVoxel.begin(), verticesVoxel.end());
                this->indices.insert(this->indices.end(), indicesVoxel.begin(), indicesVoxel.end());
            }
        }
    }
    
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &(this->vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()* sizeof(unsigned int), &(this->indices[0]) , GL_STATIC_DRAW);
}

void Chunk::drawChunk(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);

    // Draw the triangles !
    glDrawElements(
                    GL_TRIANGLES,      // mode
                    this->indices.size(), // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
}

std::vector<Voxel*> Chunk::getListeVoxels(){
    return this->listeVoxels;
}

void Chunk::setListeVoxels(std::vector<Voxel*> newListeVoxels){
    this->listeVoxels=newListeVoxels;
}
