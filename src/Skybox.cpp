#include <Skybox.hpp>

Skybox::Skybox(float size, glm::vec3 position){
    this->size = size;
    this->backBottomLeftCorner = position;
    buildSkybox();
}

void Skybox::buildSkybox(){
    //std::cout << "Construction des faces de la skybox\n";
    for (int i = 0 ; i < 6 ; i++){
        for (int h = 0; h < 2 ; h++) {
            for (int w = 0; w < 2; w++) {
                float x, y, z;
                int n = (i%2 == 0 ? 1 : -1);

                 if (i < 2){ // Faces bottom et top
                    x = this->backBottomLeftCorner[0] + (float)w * this->size;
                    y = this->backBottomLeftCorner[1] + i * this->size;
                    z = this->backBottomLeftCorner[2] + i*this->size + ((float)h * this->size)*n; 
                }else if (i == 2){ // Faces back
                    x = this->backBottomLeftCorner[0] + this->size * (1-w);
                    y = this->backBottomLeftCorner[1] + (float)h * this->size; 
                    z = this->backBottomLeftCorner[2];
                }else if (i == 3){ // Faces front
                    x = this->backBottomLeftCorner[0] + (float)w * this->size;
                    y = this->backBottomLeftCorner[1] + (float)h * this->size; 
                    z = this->backBottomLeftCorner[2] + this->size;
                }else if (i == 4){ // Face left
                    x = this->backBottomLeftCorner[0] + (i-4) * this->size;
                    y = this->backBottomLeftCorner[1] + (float)h * this->size;
                    z = this->backBottomLeftCorner[2] + (float)w * this->size; 
                }else if (i == 5){ // Face right
                    x = this->backBottomLeftCorner[0] + (i-4) * this->size;
                    y = this->backBottomLeftCorner[1] + (float)h * this->size;
                    z = this->backBottomLeftCorner[2] + this->size * (1-w); 
                }
                this->vertices.push_back(glm::vec3(x,y,z));
            }
        }

        unsigned int decalage = i*4; 
        this->indices.push_back(decalage + 3);
        this->indices.push_back(decalage + 0);
        this->indices.push_back(decalage + 2);
        this->indices.push_back(decalage + 1);
        this->indices.push_back(decalage + 0);
        this->indices.push_back(decalage + 3);
    }
}

void Skybox::loadSkybox(){
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &(this->vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()* sizeof(unsigned int), &(this->indices[0]) , GL_STATIC_DRAW);
}

void Skybox::drawSkybox(GLint programID){

    // Temporaire (pour l'application de la texture)
    int obj = 1;
    GLint location = glGetUniformLocation(programID, "objectID");
	glUniform1i(location, obj);

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