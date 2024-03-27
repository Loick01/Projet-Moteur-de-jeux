#include <Voxel.hpp>

Voxel::Voxel(){
    // Face avant
    this->vertices.push_back(glm::vec3(-0.5f, -0.5f,  0.5f)); // Bas gauche (0)
    this->vertices.push_back(glm::vec3(0.5f, -0.5f,  0.5f)); // Bas droit (1)
    this->vertices.push_back(glm::vec3(-0.5f, 0.5f,  0.5f)); // Haut gauche (2)
    this->vertices.push_back(glm::vec3(0.5f, 0.5f,  0.5f)); // Haut droit (3)
    // Face arrière
    this->vertices.push_back(glm::vec3(-0.5f, -0.5f,  -0.5f)); // Bas gauche (4)
    this->vertices.push_back(glm::vec3(0.5f, -0.5f,  -0.5f)); // Bas droit (5)
    this->vertices.push_back(glm::vec3(-0.5f, 0.5f,  -0.5f)); // Haut gauche (6)
    this->vertices.push_back(glm::vec3(0.5f, 0.5f,  -0.5f)); // Haut droit (7)

    // Définition des triangles
    std::vector<unsigned short> indices = {
        // Face avant
        0, 1, 2,
        2, 1, 3,
        // Face arrière
        5, 4, 7,
        7, 4, 6,
        // Face de gauche
        4, 0, 6,
        6, 0, 2,
        // Face de droite
        1, 5, 3,
        3, 5, 7,
        // Face dessous
        4, 5, 0,
        0, 5, 1,
        // Face dessus
        2, 3, 6,
        6, 3, 7

    };
    this->indices = indices;
}

void Voxel::loadVoxel(){
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &(this->vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned short), &(this->indices[0]) , GL_STATIC_DRAW);
}

void Voxel::drawVoxel(){
    //glUniform1i(glGetUniformLocation(programID, "objectID"),this->facesVoxel[i]->faceId);
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
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
}
/*
std::vector<Face*> Voxel::getFacesVoxel(){
    return this->facesVoxel;
}

glm::vec3 Voxel::getPoint(){
    return this->backBottomLeftCorner;
}

void Voxel::updatePoint(glm::vec3 motion){
    this->backBottomLeftCorner += motion;
}
*/