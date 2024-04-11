#include <Hud.hpp>

Hud::Hud(glm::vec2 bottomLeft,float widthPlane, float heightPlane){
    this->hotbar.vertices.push_back(bottomLeft);
    this->hotbar.vertices.push_back(glm::vec2(bottomLeft[0]+widthPlane,bottomLeft[1]));
    this->hotbar.vertices.push_back(glm::vec2(bottomLeft[0],bottomLeft[1]+heightPlane));
    this->hotbar.vertices.push_back(glm::vec2(bottomLeft[0]+widthPlane,bottomLeft[1]+heightPlane));

    this->hotbar.indices.push_back(0);
    this->hotbar.indices.push_back(3);
    this->hotbar.indices.push_back(2);
    this->hotbar.indices.push_back(0);
    this->hotbar.indices.push_back(1);
    this->hotbar.indices.push_back(3);
}

void Hud::loadHud(){
    glGenBuffers(1, &(this->hotbar.vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->hotbar.vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->hotbar.vertices.size() * sizeof(glm::vec2), &(this->hotbar.vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->hotbar.elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->hotbar.elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->hotbar.indices.size()* sizeof(unsigned int), &(this->hotbar.indices[0]) , GL_STATIC_DRAW);
}

void Hud::drawHud(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->hotbar.vertexbuffer);
    glVertexAttribPointer(
                    0,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->hotbar.elementbuffer);

    // Draw the triangles !
    glDrawElements(
                    GL_TRIANGLES,      // mode
                    this->hotbar.indices.size(), // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
}