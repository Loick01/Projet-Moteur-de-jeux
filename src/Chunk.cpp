#include <Chunk.hpp>

#define CHUNK_SIZE 32

Chunk::Chunk(glm::vec3 position, GLubyte *texels, GLint widthTexture, GLint heightTexture){
    this->position = position;
    this->buildChunk(texels, widthTexture, heightTexture);
}

void Chunk::buildChunk(GLubyte *texels, GLint widthTexture, GLint heightTexture){
    this->listeVoxels.clear();
    short compteur = 0; // Dénombre les voxels qui seront envoyés aux shaders, pour savoir quel décalage appliquer


    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){     
                //if (k < ((short)texels[((j*heightTexture)/31)*widthTexture + ((i*widthTexture)/31)]*32)/255){ // Pas sûr que ça fonctionne
                      
                                                                                                                           // Ca fait un effet sympa si on remet ça (servait à la base quand les chunks étaient remplis)
                    Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),compteur /* k*CHUNK_SIZE*CHUNK_SIZE + j*CHUNK_SIZE + i */); 
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

void Chunk::loadChunk(){
    // Peut être faudrait il stocker les sommets et les indices directement dans la classe Chunk, au lieu de les récupérer pour chaque voxel
    for (int i = 0 ; i < this->listeVoxels.size() ; i++){
        if (listeVoxels[i]->getVisible()){
            std::vector<glm::vec3> verticesVoxel = listeVoxels[i]->getVertices();
            std::vector<unsigned int> indicesVoxel = listeVoxels[i]->getIndices();
            this->vertices.insert(this->vertices.end(), verticesVoxel.begin(), verticesVoxel.end());
            this->indices.insert(this->indices.end(), indicesVoxel.begin(), indicesVoxel.end());
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
