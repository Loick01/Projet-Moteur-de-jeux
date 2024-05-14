#include <Hud.hpp>

PlaneHud Hud::createPlaneHud(glm::vec2 refPoint, float widthPlane, float heightPlane, int decalage){ // refPoint est le point en bas à gauche du plan

    std::vector<glm::vec2> verticesPlane;
    std::vector<unsigned int> indicesPlane;

    // Ajout des 4 sommets
    verticesPlane.push_back(refPoint);
    verticesPlane.push_back(glm::vec2(refPoint[0]+widthPlane,refPoint[1]));
    verticesPlane.push_back(glm::vec2(refPoint[0],refPoint[1]+heightPlane));
    verticesPlane.push_back(glm::vec2(refPoint[0]+widthPlane,refPoint[1]+heightPlane));

    // Ajout des 6 indices
    indicesPlane.push_back(decalage + 0);
    indicesPlane.push_back(decalage + 3);
    indicesPlane.push_back(decalage + 2);
    indicesPlane.push_back(decalage + 0);
    indicesPlane.push_back(decalage + 1);
    indicesPlane.push_back(decalage + 3);

    PlaneHud planeRes = {verticesPlane, indicesPlane};
    return planeRes;
}

Hud::Hud(int screen_width, int screen_height){
    this->screen_width = screen_width; // On en a besoin dans d'autres fonctions, donc on le met en attribut de la classe (idem pour hotbarWidth)
    this->hotbarWidth = 724.0f; 
    float hotbarHeight = 84.0f;
    float selectSize = 92.0f;
    float cursorSize =36.0f;
    this->hotbar = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 ,10.0),hotbarWidth,hotbarHeight,0);
    this->select = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 - 4.0,6.0),selectSize,selectSize,4); // Par défaut, le sélecteur est placé sur le premier élément de la hotbar
    this->cursor = createPlaneHud(glm::vec2(screen_width/2.0 - cursorSize/2.0 ,screen_height/2.0 - cursorSize/2.0),cursorSize,cursorSize,8);
    this->lifeLine = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 + 4.0 , 20.0 + hotbarHeight),hotbarWidth/2 - 16.0,14.0,12);
    this->staminaLine = createPlaneHud(glm::vec2(screen_width/2.0+14.0 ,20.0 + hotbarHeight),hotbarWidth/2-18.0,14.0,16);
    this->lifeBar = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 ,20.0 + hotbarHeight),hotbarWidth/2-10.0,16.0,20);
    this->staminaBar = createPlaneHud(glm::vec2(screen_width/2.0+10.0 ,20.0 + hotbarHeight),hotbarWidth/2-10.0,16.0,24);

    this->elements.push_back(this->hotbar);
    this->elements.push_back(this->select);
    this->elements.push_back(this->cursor);
    this->elements.push_back(this->lifeLine);
    this->elements.push_back(this->staminaLine);
    this->elements.push_back(this->lifeBar);
    this->elements.push_back(this->staminaBar);

    for (int i = 0 ; i < 9 ; i++){ // On génère les 9 emplacements de la hotbar
        PlaneHud ei = createPlaneHud(glm::vec2(screen_width/2.0 - hotbarWidth/2.0 + 80.0 * i + 8.0, 18.0),68,68,this->elements.size()*4);
        this->elements.push_back(ei);
    }
}

Hud::~Hud(){
    std::cout << "Destruction de Hud\n";
    glDeleteBuffers(1, &(this->vertexbuffer));
    glDeleteBuffers(1, &(this->elementbuffer));
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

void Hud::updateLife(float new_life){
    glDeleteBuffers(1, &(this->vertexbuffer)); // Ne pas oublier de supprimer le précédent buffer
    float new_x = this->screen_width/2.0 - this->hotbarWidth/2.0 + (new_life*(this->hotbarWidth-16.0)/200);
    this->elements[3].vertices[1].x = new_x;
    this->elements[3].vertices[3].x = new_x;

    std::vector<glm::vec2> acc_vertices;
    for (int i = 0 ; i < this->elements.size() ; i++){
        for (int v = 0 ; v < 4 ; v++){
            acc_vertices.push_back(elements[i].vertices[v]);
        }
    }

    // On prend en compte les nouveaux sommets
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, acc_vertices.size() * sizeof(glm::vec2), &(acc_vertices[0]), GL_STATIC_DRAW);
}

void Hud::updateStamina(float new_stamina){
    glDeleteBuffers(1, &(this->vertexbuffer)); // Ne pas oublier de supprimer le précédent buffer
    float new_x = this->screen_width/2.0+14.0 + (new_stamina*this->hotbarWidth/200)-18.0;
    this->elements[4].vertices[1].x = new_x;
    this->elements[4].vertices[3].x = new_x;

    std::vector<glm::vec2> acc_vertices;
    for (int i = 0 ; i < this->elements.size() ; i++){
        for (int v = 0 ; v < 4 ; v++){
            acc_vertices.push_back(elements[i].vertices[v]);
        }
    }

    // On prend en compte les nouveaux sommets
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, acc_vertices.size() * sizeof(glm::vec2), &(acc_vertices[0]), GL_STATIC_DRAW);
}