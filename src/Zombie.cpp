#include <Zombie.hpp>


Zombie::Zombie(int nodeID, glm::vec3 pos, float speedEntity){
    this->node = new Node;
    this->node->nodeID = nodeID;
    this->node->transformation = new Transform();

    this->createZombie(this->node,pos);
    this->speedEntity = speedEntity;
}

Zombie::~Zombie(){
    std::cout << "Destructeur de Zombie\n";
    // Il faudra mettre tout ce qui concerne Node dans une classe pour faire une destruction récursive comme on le fait d'habitude
    glDeleteBuffers(1, &(this->node->vertexbuffer));
    glDeleteBuffers(1, &(this->node->elementbuffer));
    for (int i = 0 ; i < this->node->fils.size() ; i++){
        glDeleteBuffers(1, &(this->node->fils[i]->vertexbuffer));
        glDeleteBuffers(1, &(this->node->fils[i]->elementbuffer));
    }
}

void Zombie::loadZombie(){
    this->loadBufferNode(this->node);
}

void Zombie::drawZombie(GLuint programID_Entity){
    this->sendNodeToShader(this->node,programID_Entity,glm::mat4(1.0f));
}

void Zombie::setPave(Node* node, glm::vec3 dimensions, glm::vec3 position) {
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
    glGenBuffers(1, &(node->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, node->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, node->indexed_vertices.size() * sizeof(glm::vec3), &(node->indexed_vertices[0]), GL_STATIC_DRAW);

    glGenBuffers(1, &(node->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, node->indices.size() * sizeof(unsigned short), &(node->indices[0]) , GL_STATIC_DRAW);
    for(int i=0;i<node->fils.size();i++){
        loadBufferNode(node->fils[i]);
    }
}

void Zombie::sendNodeToShader(Node *node,GLuint programID_Entity,glm::mat4 parent){
    glm::mat4 matTransfo = parent*node->transformation->getTransfoMat4();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, node->vertexbuffer);
    glUniformMatrix4fv(glGetUniformLocation(programID_Entity, "Model"), 1, GL_FALSE, &(matTransfo[0][0]));
    glUniform1i(glGetUniformLocation(programID_Entity,"nodeID"), node->nodeID);
    
    
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node->elementbuffer);
    // Draw the triangles !
    glDrawElements(
                GL_TRIANGLES,      // mode
                node->indices.size(),    // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0           // element array buffer offset
                );

    glDisableVertexAttribArray(0);
    for(int i=0;i<node->fils.size();i++){
        sendNodeToShader(node->fils[i],programID_Entity,matTransfo);
    }
}

void Zombie::createZombie(Node* node, glm::vec3 position){
    Node *headZombie = new Node;
    Node *chestZombie = new Node;
    Node *leftArmZombie = new Node;
    Node *rightArmZombie = new Node;
    Node *leftLegZombie = new Node;
    Node *rightLegZombie = new Node;
    headZombie->nodeID=0;
    chestZombie->nodeID=1;
    leftArmZombie->nodeID=2;
    rightArmZombie->nodeID=3;
    leftLegZombie->nodeID=4;
    rightLegZombie->nodeID=5;
    this->setPave(chestZombie,glm::vec3(0.6,0.8,0.3), glm::vec3(0,0.8,0)+ position);
    this->setPave(headZombie,glm::vec3(0.6,0.6,0.6), glm::vec3(0,1.5,0)+ position);
    this->setPave(leftArmZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(0.45,0.8,0)+ position);
    this->setPave(rightArmZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(-0.45,0.8,0)+ position);
    this->setPave(leftLegZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(0.15,0.05,0)+ position);
    this->setPave(rightLegZombie,glm::vec3(0.3,0.8,0.3), glm::vec3(-0.15,0.05,0)+ position);

    chestZombie->center=glm::vec3(0,0.8,0)+ position;
    headZombie->center=glm::vec3(0,1.5,0)+ position;
    rightArmZombie->center=glm::vec3(0.45,0.8,0)+ position;
    leftArmZombie->center=glm::vec3(0.15,0,0)+ position;
    rightLegZombie->center=glm::vec3(0.15,0,0)+ position;
    leftLegZombie->center=glm::vec3(-0.15,0,0)+ position;

    chestZombie->transformation = new Transform();
    headZombie->transformation = new Transform();
    rightArmZombie->transformation = new Transform();
    leftArmZombie->transformation = new Transform();
    rightLegZombie->transformation = new Transform();
    leftLegZombie->transformation = new Transform();

    node->fils.push_back(chestZombie);
    node->fils.push_back(headZombie);
    node->fils.push_back(leftArmZombie);
    node->fils.push_back(rightArmZombie);
    node->fils.push_back(leftLegZombie);
    node->fils.push_back(rightLegZombie);
}

void Zombie::walk(Node* node,float angle,float deltaTime){ // Le paramètre node doit être un zombie dans le graphe de scène
    float angleLeg1 = sin(angle);
    float angleLeg2 = sin(angle+M_PI);

    // Les 2 bras ont la même transformation
    glm::mat4 matTransfoArm = glm::translate(glm::mat4(1.0f),node->fils[2]->center+glm::vec3(0.45,1.2,0));
    matTransfoArm = glm::rotate(matTransfoArm,(float)-M_PI/1.9f,glm::vec3(1.f,0.0f,0.0f));
    matTransfoArm = glm::translate(matTransfoArm,-node->fils[2]->center-glm::vec3(0.45,1.2,0));
    
    glm::mat4 matTransfoLeg1 = glm::translate(glm::mat4(1.0f),node->fils[4]->center+glm::vec3(0.15,0.4,0));
    matTransfoLeg1 = glm::rotate(matTransfoLeg1, angleLeg1,glm::vec3(1.f,0.0f,0.0f));
    matTransfoLeg1 = glm::translate(matTransfoLeg1,-node->fils[4]->center-glm::vec3(0.15,0.4,0));

    glm::mat4 matTransfoLeg2 = glm::translate(glm::mat4(1.0f),node->fils[5]->center+glm::vec3(0.15,0.4,0));
    matTransfoLeg2 = glm::rotate(matTransfoLeg2, angleLeg2,glm::vec3(1.f,0.0f,0.0f));
    matTransfoLeg2 = glm::translate(matTransfoLeg2,-node->fils[5]->center-glm::vec3(0.15,0.4,0));

    node->transformation->addVelocity(glm::vec3(0,0,this->speedEntity*deltaTime));
    node->fils[4]->transformation = new Transform(matTransfoLeg1);
    node->fils[5]->transformation = new Transform(matTransfoLeg2);
    node->fils[3]->transformation = new Transform(matTransfoArm);
    node->fils[2]->transformation = new Transform(matTransfoArm);
}

void Zombie::reset(Node* node){ // Le paramètre node doit être un zombie dans le graphe de scène
 
    float angle = sin(0);

    glm::mat4 matTransfoArm = glm::translate(glm::mat4(1.0f),node->fils[2]->center+glm::vec3(0.45,1.2,0));
    matTransfoArm = glm::rotate(matTransfoArm,angle,glm::vec3(1.f,0.0f,0.0f));
    matTransfoArm = glm::translate(matTransfoArm,-node->fils[2]->center-glm::vec3(0.45,1.2,0));
    
    glm::mat4 matTransfoLeg1 = glm::translate(glm::mat4(1.0f),node->fils[4]->center+glm::vec3(0.15,0.4,0));
    matTransfoLeg1 = glm::rotate(matTransfoLeg1,angle,glm::vec3(1.f,0.0f,0.0f));
    matTransfoLeg1 = glm::translate(matTransfoLeg1,-node->fils[4]->center-glm::vec3(0.15,0.4,0));

    glm::mat4 matTransfoLeg2 = glm::translate(glm::mat4(1.0f),node->fils[5]->center+glm::vec3(0.15,0.4,0));
    matTransfoLeg2 = glm::rotate(matTransfoLeg2,angle,glm::vec3(1.f,0.0f,0.0f));
    matTransfoLeg2 = glm::translate(matTransfoLeg2,-node->fils[5]->center-glm::vec3(0.15,0.4,0));
            
    node->fils[4]->transformation = new Transform(matTransfoLeg1);
    node->fils[5]->transformation = new Transform(matTransfoLeg2);
    node->fils[3]->transformation = new Transform(matTransfoArm);
    node->fils[2]->transformation = new Transform(matTransfoArm);
}

void Zombie::attack(Node* node, bool *attack, float *accumulateurAnimation, float deltaTime){ // Le paramètre node doit être un zombie dans le graphe de scène
    float angle = -M_PI/1.9; // Angle en radians (pour toutes les animations il vaudrait mieux que ce soit le cas pour ne pas confondre)

    // Les 2 bras ont la même transformation
    glm::mat4 matTransfoArm = glm::translate(glm::mat4(1.0f),node->fils[2]->center+glm::vec3(0.45,1.2,0));
    matTransfoArm = glm::rotate(matTransfoArm,angle,glm::vec3(1.f,0.0f,0.0f));
    matTransfoArm = glm::translate(matTransfoArm,-node->fils[2]->center-glm::vec3(0.45,1.2,0));
            
    node->fils[3]->transformation = new Transform(matTransfoArm);
    node->fils[2]->transformation = new Transform(matTransfoArm);
    
    if(*accumulateurAnimation>0.8f){ // Ici, 0.8 c'est la durée de l'animation (sachant qu'on rajoute deltaTime à l'accumulateur, ce qui garantit l'indépendance entre temps d'animation et nombre de FPS)
        *attack=false;
        *accumulateurAnimation=0.0f;
    }else{
        *accumulateurAnimation += deltaTime;
    }
}

// Peut être revoir l'animation de mort si on a le temps (pour faire plus joli)
void Zombie::die(Node* node, bool *die, float *accumulateurAnimation, float deltaTime){ // Le paramètre node doit être un zombie dans le graphe de scène
    glm::mat4 matTransAll = glm::translate(node->transformation->getTransfoMat4(),node->fils[0]->center-glm::vec3(0,1.6,0));
    matTransAll = glm::rotate(matTransAll,deltaTime*4,glm::vec3(0.f,0.0f,1.0f));
    matTransAll = glm::translate(matTransAll,-node->fils[0]->center+glm::vec3(0,1.6,0));
    
    if(*accumulateurAnimation<0.02){
        matTransAll = glm::translate(matTransAll,glm::vec3(0,deltaTime*25,0));
        *accumulateurAnimation += deltaTime;
    }
    if(*accumulateurAnimation<0.4){
        node->transformation = new Transform(matTransAll);
        *accumulateurAnimation += deltaTime;
    }
    if(*accumulateurAnimation>0.8){
        *die=false;
        *accumulateurAnimation=0.0f;
    }
}

Node* Zombie::getRootNode(){
    return this->node;
}