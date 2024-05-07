#include <Zombie.hpp>


Zombie::Zombie(int ID,glm::vec3 pos){

    this->node = new Node;
    this->node->ID = ID;
    this->node->transformation = new Transform();

    this->hp=20;
    this->speed=10;
    this->att=10;

    this->createZombie(this->node,pos);

}

Zombie::~Zombie(){ // Si on laisse un destructeur vide, valgrind aime pas ça
    std::cout << "Destructeur de Zombie\n";
}

void Zombie::loadZombie(){
    this->loadBufferNode(this->node);
}

void Zombie::drawZombie(GLuint programID_Entity){
    this->sendNodeToBuffer(this->node,programID_Entity,glm::mat4(1));
}

void Zombie::setPave(Node* node, glm::vec3 dimensions, glm::vec3 position) {
    // Effacer les anciens éléments
    node->indices.clear();
    node->indexed_vertices.clear();

    // Dimensions du pavé
    float dimX = dimensions.x;
    float dimY = dimensions.y;
    float dimZ = dimensions.z;

    for (int i = 0 ; i < 6 ; i++){
        for (int h = 0; h < 2 ; h++) {
            for (int w = 0; w < 2; w++) {
                float x, y, z;

                if (i==0){ // Faces bottom
                    x = -dimX/2 + w*dimX + position[0];
                    y = -dimY/2 + position[1];
                    z = -dimZ/2 + h*dimZ + position[2];
                }else if (i==1){ // Faces top
                    x = -dimX/2 + w*dimX + position[0];
                    y = dimY/2 + position[1];
                    z = -dimZ/2 + (1-h)*dimZ + position[2];
                }else if (i == 2){ // Faces back
                    x = -dimX/2 + (1-w)*dimX + position[0];
                    y = -dimY/2 + h*dimY + position[1];
                    z = -dimZ/2 + position[2];
                }else if (i == 3){ // Faces front
                    x = -dimX/2 + w*dimX + position[0];
                    y = -dimY/2 + h*dimY + position[1];
                    z = dimZ/2 + position[2];
                }else if (i == 4){ // Face left
                    x = -dimX/2 + position[0];
                    y = -dimY/2 + h*dimY + position[1];
                    z = -dimZ/2 + w*dimZ + position[2];
                }else if (i == 5){ // Face right
                    x = dimX/2 + position[0];
                    y = -dimY/2 + h*dimY + position[1];
                    z = -dimZ/2 + (1-w)*dimZ + position[2]; 
                }
                node->indexed_vertices.push_back(glm::vec3(x,y,z));
            }
        }
    }

    // Indices pour les triangles des faces du pavé
    node->indices.insert(node->indices.end(), {
        // Face du bas (y négatif)
        0,1,2,
        2,1,3,

        // Face du haut (y positif)
        4,7,6,
        5,7,4,

        // Face arrière (z négatif)
        11,8,9,
        10,8,11,

        // Face avant (z positif)
        12,13,14,
        14,13,15,

        // Face de gauche
        16,17,18,
        18,17,19,

        // Face de droite
        23,20,21,
        22,20,23
    });
}

void Zombie::loadBufferNode(Node *node){
    glGenBuffers(1, &(node->vbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, node->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, node->indexed_vertices.size() * sizeof(glm::vec3), &(node->indexed_vertices[0]), GL_STATIC_DRAW);

    glGenBuffers(1, &(node->ebuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node->ebuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, node->indices.size() * sizeof(unsigned short), &(node->indices[0]) , GL_STATIC_DRAW);
    for(int i=0;i<node->son.size();i++){
        loadBufferNode(node->son[i]);
    }
}

void Zombie::sendNodeToBuffer(Node *node,GLuint programID_Entity,glm::mat4 parent){
    glm::mat4 matI = glm::mat4(1);
    matI=parent*node->transformation->getMatrix4();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, node->vbuffer);
    glUniformMatrix4fv(glGetUniformLocation(programID_Entity, "Model"), 1, GL_FALSE, &matI[0][0]);
    glUniform1i(glGetUniformLocation(programID_Entity,"ID"), node->ID);
    
    
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node->ebuffer);
    // Draw the triangles !
    glDrawElements(
                GL_TRIANGLES,      // mode
                node->indices.size(),    // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0           // element array buffer offset
                );

    glDisableVertexAttribArray(0);
    for(int i=0;i<node->son.size();i++){
        sendNodeToBuffer(node->son[i],programID_Entity,matI);
    }
}

void Zombie::createZombie(Node* node, glm::vec3 position){
    Node *headZombie = new Node;
    Node *chestZombie = new Node;
    Node *leftArmZombie = new Node;
    Node *rightArmZombie = new Node;
    Node *leftLegZombie = new Node;
    Node *rightLegZombie = new Node;
    headZombie->ID=0;
    chestZombie->ID=1;
    leftArmZombie->ID=2;
    rightArmZombie->ID=3;
    leftLegZombie->ID=4;
    rightLegZombie->ID=5;
    this->setPave(chestZombie,glm::vec3(0.6,0.8,0.3), glm::vec3(0,0.8,0)+ position);
    this->setPave(headZombie,glm::vec3(0.6,0.6,0.6), glm::vec3(0,1.5,0)+ position);
    this->setPave(leftArmZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(0.45,0.8,0)+ position);
    this->setPave(rightArmZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(-0.45,0.8,0)+ position);
    this->setPave(leftLegZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(0.15,0.05,0)+ position);
    this->setPave(rightLegZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(-0.15,0.05,0)+ position);

    chestZombie->centerNode=glm::vec3(0,0.8,0)+ position;
    headZombie->centerNode=glm::vec3(0,1.5,0)+ position;
    rightArmZombie->centerNode=glm::vec3(0.45,0.8,0)+ position;
    leftArmZombie->centerNode=glm::vec3(0.15,0,0)+ position;
    rightLegZombie->centerNode=glm::vec3(0.15,0,0)+ position;
    leftLegZombie->centerNode=glm::vec3(-0.15,0,0)+ position;

    chestZombie->transformation = new Transform(); // Appliquer la rotation à la matrice
    headZombie->transformation = new Transform(); // Appliquer la rotation à la matrice
    rightArmZombie->transformation = new Transform(); // Appliquer la rotation à la matrice
    leftArmZombie->transformation = new Transform(); // Appliquer la rotation à la matrice
    rightLegZombie->transformation = new Transform(); // Appliquer la rotation à la matrice
    leftLegZombie->transformation = new Transform(); // Appliquer la rotation à la matrice

    node->son.push_back(chestZombie);
    node->son.push_back(headZombie);
    node->son.push_back(leftArmZombie);
    node->son.push_back(rightArmZombie);
    node->son.push_back(leftLegZombie);
    node->son.push_back(rightLegZombie);
}

void Zombie::walk(Node* node,float angle,float deltatime){ // Le paramètre node doit être un zombie dans le graphe de scène
    float angle2;
    float angleArm;
    angle2 = sin(angle/20+(M_PI))*5400*deltatime;;
    angle = sin(angle/20)*5400*deltatime;
    
    glm::mat4 transfoLeg = glm::mat4(1);
    glm::mat4 transfoLeg2 = glm::mat4(1);
    glm::mat4 transfoArm = glm::mat4(1);

    transfoArm = glm::translate(transfoArm,node->son[2]->centerNode+glm::vec3(0.45,1.2,0));
    transfoArm = glm::rotate(transfoArm,80.0f,glm::vec3(1.f,0.0f,0.0f));
    transfoArm = glm::translate(transfoArm,-node->son[2]->centerNode-glm::vec3(0.45,1.2,0));
    
    transfoLeg = glm::translate(transfoLeg,node->son[4]->centerNode+glm::vec3(0.15,0.4,0));
    transfoLeg = glm::rotate(transfoLeg, glm::radians(angle),glm::vec3(1.f,0.0f,0.0f));
    transfoLeg = glm::translate(transfoLeg,-node->son[4]->centerNode-glm::vec3(0.15,0.4,0));

    transfoLeg2 = glm::translate(transfoLeg2,node->son[5]->centerNode+glm::vec3(0.15,0.4,0));
    transfoLeg2 = glm::rotate(transfoLeg2, glm::radians(angle2),glm::vec3(1.f,0.0f,0.0f));
    transfoLeg2 = glm::translate(transfoLeg2,-node->son[5]->centerNode-glm::vec3(0.15,0.4,0));
    node->transformation->addVelocity(glm::vec3(0,0,0.01f));
            
    node->son[4]->transformation = new Transform(transfoLeg);
    node->son[5]->transformation = new Transform(transfoLeg2);
    node->son[3]->transformation = new Transform(transfoArm);
    node->son[2]->transformation = new Transform(transfoArm);
}

void Zombie::reset(Node* node){ // Le paramètre node doit être un zombie dans le graphe de scène
 
    float angle = sin(0);
    
    glm::mat4 transfoLeg = glm::mat4(1);
    glm::mat4 transfoLeg2 = glm::mat4(1);
    glm::mat4 transfoArm = glm::mat4(1);

    transfoArm = glm::translate(transfoArm,node->son[2]->centerNode+glm::vec3(0.45,1.2,0));
    transfoArm = glm::rotate(transfoArm,angle,glm::vec3(1.f,0.0f,0.0f));
    transfoArm = glm::translate(transfoArm,-node->son[2]->centerNode-glm::vec3(0.45,1.2,0));
    
    transfoLeg = glm::translate(transfoLeg,node->son[4]->centerNode+glm::vec3(0.15,0.4,0));
    transfoLeg = glm::rotate(transfoLeg,angle,glm::vec3(1.f,0.0f,0.0f));
    transfoLeg = glm::translate(transfoLeg,-node->son[4]->centerNode-glm::vec3(0.15,0.4,0));

    transfoLeg2 = glm::translate(transfoLeg2,node->son[5]->centerNode+glm::vec3(0.15,0.4,0));
    transfoLeg2 = glm::rotate(transfoLeg2,angle,glm::vec3(1.f,0.0f,0.0f));
    transfoLeg2 = glm::translate(transfoLeg2,-node->son[5]->centerNode-glm::vec3(0.15,0.4,0));
            
    node->son[4]->transformation = new Transform(transfoLeg);
    node->son[5]->transformation = new Transform(transfoLeg2);
    node->son[3]->transformation = new Transform(transfoArm);
    node->son[2]->transformation = new Transform(transfoArm);
}

void Zombie::attack(Node* node,bool *attack,int *time_Animation){ // Le paramètre node doit être un zombie dans le graphe de scène
 
    float angle = 79.8f;
    
    glm::mat4 transfoLeg = glm::mat4(1);
    glm::mat4 transfoLeg2 = glm::mat4(1);
    glm::mat4 transfoArm = glm::mat4(1);

    transfoArm = glm::translate(transfoArm,node->son[2]->centerNode+glm::vec3(0.45,1.2,0));
    transfoArm = glm::rotate(transfoArm,angle,glm::vec3(1.f,0.0f,0.0f));
    transfoArm = glm::translate(transfoArm,-node->son[2]->centerNode-glm::vec3(0.45,1.2,0));
    
            
    node->son[3]->transformation = new Transform(transfoArm);
    node->son[2]->transformation = new Transform(transfoArm);
    
    if(*time_Animation>50){
        *attack=false;
        *time_Animation=0;
    }
}

void Zombie::die(Node* node,bool *die,int *time_Animation){ // Le paramètre node doit être un zombie dans le graphe de scène
    glm::mat4 transfoNode = node->transformation->getMatrix4();
    transfoNode = glm::translate(transfoNode,node->son[0]->centerNode-glm::vec3(0,1.6,0));
    transfoNode = glm::rotate(transfoNode,0.02f,glm::vec3(0.f,0.0f,1.0f));
    transfoNode = glm::translate(transfoNode,-node->son[0]->centerNode+glm::vec3(0,1.6,0));
    
    if(*time_Animation<3){
        transfoNode = glm::translate(transfoNode,glm::vec3(0,0.2,0));;
    }
    if(*time_Animation<70){
        node->transformation = new Transform(transfoNode);
    }
    if(*time_Animation>140){
        *die=false;
        *time_Animation=0;
    }
}

Node* Zombie::getParentNode(){
    return this->node;
}