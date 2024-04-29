#include <Hud.hpp>

PlaneHud Hud::createPlaneHud(glm::vec2 refPoint, float widthPlane, float heightPlane, int decalage){ // refPoint est le point en bas à gauche du plan
    PlaneHud planeRes;
    planeRes.visible=true;

    // Ajout des 4 sommets
    planeRes.vertices.push_back(refPoint);
    planeRes.vertices.push_back(glm::vec2(refPoint[0]+widthPlane,refPoint[1]));
    planeRes.vertices.push_back(glm::vec2(refPoint[0],refPoint[1]+heightPlane));
    planeRes.vertices.push_back(glm::vec2(refPoint[0]+widthPlane,refPoint[1]+heightPlane));

    // Ajout des 6 indices
    planeRes.indices.push_back(decalage + 0);
    planeRes.indices.push_back(decalage + 3);
    planeRes.indices.push_back(decalage + 2);
    planeRes.indices.push_back(decalage + 0);
    planeRes.indices.push_back(decalage + 1);
    planeRes.indices.push_back(decalage + 3);

    return planeRes;
}

Hud::Hud(int screen_width, int screen_height){
    float hotbarWidth = 724.0f;
    float hotbarHeight = 84.0f;
    float selectSize = 92.0f;
    float cursorSize =36.0f;
    float inventoryWidth = 352*1.5;
    float inventoryHeight = 332*1.5;
    this->select = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 - 4.0,6.0),selectSize,selectSize,0); // Par défaut, le sélecteur est placé sur le premier élément de la hotbar
    this->hotbar = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 ,10.0),hotbarWidth,hotbarHeight,4);
    this->cursor = createPlaneHud(glm::vec2(screen_width/2.0 - cursorSize/2.0 ,screen_height/2.0 - cursorSize/2.0),cursorSize,cursorSize,8);
    this->inventory=createPlaneHud(glm::vec2(screen_width/2.0 - inventoryWidth/2.0 ,135.0),inventoryWidth,inventoryHeight,2);

    this->elements.push_back(this->select);
    this->elements.push_back(this->hotbar);
    this->elements.push_back(this->cursor);
    this->elements.push_back(this->inventory);

    for (int i = 0 ; i < 9 ; i++){ // On génère les 9 emplacements de la hotbar
        PlaneHud ei = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 + 80.0 * i + 8.0, 18.0),68,68,12+i*4);
        this->elements.push_back(ei);
    }
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