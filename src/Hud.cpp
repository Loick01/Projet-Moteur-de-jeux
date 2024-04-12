#include <Hud.hpp>

Hud::Hud(glm::vec2 bottomLeft,float widthPlane, float heightPlane/*, float widthSelect, float heightSelect*/){
    PlaneHud hotbar;
    hotbar.vertices.push_back(bottomLeft);
    hotbar.vertices.push_back(glm::vec2(bottomLeft[0]+widthPlane,bottomLeft[1]));
    hotbar.vertices.push_back(glm::vec2(bottomLeft[0],bottomLeft[1]+heightPlane));
    hotbar.vertices.push_back(glm::vec2(bottomLeft[0]+widthPlane,bottomLeft[1]+heightPlane));

    hotbar.indices.push_back(0);
    hotbar.indices.push_back(3);
    hotbar.indices.push_back(2);
    hotbar.indices.push_back(0);
    hotbar.indices.push_back(1);
    hotbar.indices.push_back(3);

    this->elements.push_back(hotbar);

    /* Temporaire
    PlaneHud select;
    select.vertices.push_back(bottomLeft);
    select.vertices.push_back(glm::vec2(bottomLeft[0]+widthSelect,bottomLeft[1]));
    select.vertices.push_back(glm::vec2(bottomLeft[0],bottomLeft[1]+heightSelect));
    select.vertices.push_back(glm::vec2(bottomLeft[0]+widthSelect,bottomLeft[1]+heightSelect));

    select.indices.push_back(4);
    select.indices.push_back(7);
    select.indices.push_back(6);
    select.indices.push_back(4);
    select.indices.push_back(5);
    select.indices.push_back(7);

    this->elements.push_back(select);
    */

    this->nbIndices = 0;
}

void Hud::loadHud(){
    std::vector<glm::vec2> acc_vertices;
    std::vector<unsigned int> acc_indices;
    for (int i = 0 ; i < this->elements.size() ; i++){
        for (int v = 0 ; v < 4 ; v++){
            acc_vertices.push_back(elements[i].vertices[v]);
        }
        for (int ind = 0 ; ind < 6 ; ind++){
            acc_indices.push_back(elements[i].indices[ind]);
            this->nbIndices++;
        }
    }
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, acc_vertices.size() * sizeof(glm::vec2), &(acc_vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, acc_indices.size()* sizeof(unsigned int), &(acc_indices[0]) , GL_STATIC_DRAW);
}

void Hud::drawHud(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glVertexAttribPointer(
                    0,                  // attribute
                    2,                  // size
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
                    this->nbIndices, // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
}