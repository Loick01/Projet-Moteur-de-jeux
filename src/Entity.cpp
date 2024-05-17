#include <Entity.hpp>

Entity::Entity(int type, int nodeID, glm::vec3 pos, float speedEntity,float entityHeight,float entityWidth,float entityLenght, float vitesseRotationLeg, float vitesseRotation, float damageEntity, float areaDetection){ // type = 0 pour zombie, 1 pour cochon
    this->node = new Node;
    this->node->nodeID = nodeID;
    this->node->transformation = new Transform();
    this->speedEntity = speedEntity;
    this->type=type;
    if(this->type==0)this->agent = new Agent(0.0f);
    else if (this->type==1)this->agent = new Agent(M_PI/2);
    this->vitesseRotationLeg = vitesseRotationLeg;
    this->vitesseRotation = vitesseRotation;
    this->damageEntity = damageEntity;
    this->areaDetection = areaDetection;
    this->nearPlayer = false;

    if (type==0){
        this->createZombie(this->node,pos);
        this->hitbox = new Hitbox(pos+glm::vec3(0,-0.4,0), entityHeight, entityWidth, 21.0f, 7.5f);
        this->life = 10.0f; // 10 pv pour le zombie
    }else if (type == 1){
        this->createCochon(this->node,pos);
        this->hitbox = new Hitbox(pos+glm::vec3(0,-0.4,0), entityHeight, entityWidth,entityLenght,21.0f, 7.5f);
        this->life = 8.0f; // 8 pv pour le zombie
    }
}

Entity::~Entity(){
    std::cout << "Destructeur de Entity\n";
    // Il faudra mettre tout ce qui concerne Node dans une classe pour faire une destruction récursive comme on le fait d'habitude
    glDeleteBuffers(1, &(this->node->vertexbuffer));
    glDeleteBuffers(1, &(this->node->elementbuffer));
    for (int i = 0 ; i < this->node->fils.size() ; i++){
        glDeleteBuffers(1, &(this->node->fils[i]->vertexbuffer));
        glDeleteBuffers(1, &(this->node->fils[i]->elementbuffer));
    }
    delete this->agent;
    delete this->hitbox;
}

void Entity::loadEntity(){
    this->loadBufferNode(this->node);
}


float Entity::drawEntity(GLuint programID_Entity, int numEntity, float deltaTime,TerrainControler *terrainControler, Player *p){
    float damageFromEntity = 0.0f;

    float distanceToPlayer = sqrt(pow((this->hitbox->getBottomPoint()[0]-p->getHitbox()->getBottomPoint()[0]),2) + pow((this->hitbox->getBottomPoint()[1]-p->getHitbox()->getBottomPoint()[1]),2) +pow((this->hitbox->getBottomPoint()[2]-p->getHitbox()->getBottomPoint()[2]),2));
    if (distanceToPlayer < 3.0){
        this->nearPlayer = true;
    }else{
        this->nearPlayer = false;
    }

    if(!(this->agent->getIsAttacking())){
        if(!(this->agent->getIsMoving()) && rand()%300==0){
            this->agent->createMouvement(glm::vec3(-1.0f + ((rand()%21)/10.0f),0,-1.0f + ((rand()%21)/10.0f)));
        }else if(this->agent->getIsMoving()){
            glm::vec3 cross_point;
            // Pour vérifier la collision latérale, on ne regarde que dans la direction vers laquelle l'entité essaie d'aller (c'est pas très précis, mais ça facilite grandement la chose, et c'est plutôt satisfaisant)
            if(this->hitbox->getLateralMovePossible(false,1,this->agent->getDirection(),glm::vec3(0,1,0),terrainControler,&cross_point)){
                if(this->type==0){
                    this->walk(this->node,this->agent->getAngleForLeg(),deltaTime);
                }   
                if(this->type==1){
                    this->walkCochon(this->node,this->agent->getAngleForLeg(),deltaTime);
                }   
            }
                if(this->agent->getAngleOfView() < this->agent->getAngleToReach() - 0.1){
                    this->rotateEntity(this->vitesseRotation*deltaTime);
                }else if(this->agent->getAngleOfView() > this->agent->getAngleToReach() + 0.1){
                    this->rotateEntity(-(this->vitesseRotation)*deltaTime);
                }
            if(this->agent->getRemainingTime() <= 0){
                this->agent->setIsMoving(false);
                this->reset(this->node);
            }
            this->agent->addToAngleForLeg(this->vitesseRotationLeg*deltaTime);
            this->agent->timePass(deltaTime);
        }

        // Seul le zombie peut attaquer
        if(distanceToPlayer < this->areaDetection && this->type == 0){
            if(distanceToPlayer < 1.0f){
                this->agent->setIsMoving(false);
                this->agent->resetAccumulateur();
                this->reset(this->node);
                // Ici on modifie la force avec laquelle les entités (ici les zombies) éjectent le joueur
                p->getHitbox()->resetJumpForce(1);
                p->getHitbox()->setCanJump(false);
                damageFromEntity = this->damageEntity;
                this->agent->setIsAttacking(true);
                    
            }else{
                glm::vec3 d = -glm::vec3(this->hitbox->getBottomPoint()[0]-p->getHitbox()->getBottomPoint()[0],0,this->hitbox->getBottomPoint()[2]-p->getHitbox()->getBottomPoint()[2]);
                this->agent->createMouvement(d);
            }
        }
            
    }else{
        this->attack(this->node,deltaTime);
    }

    
    glm::vec3 initialPos = this->hitbox->getBottomPoint();
    float damage = this->hitbox->checkTopAndBottomCollision(false,deltaTime,terrainControler);
    if (damage == -1.0f){
        this->takeDamage(this->getLife() + 1.0f); // On est sûr de tuer l'entité si elle tombe dans le vide
    }else if (damage > 0.0f){
        this->takeDamage(damage);
    }
    glm::vec3 targetPos = this->hitbox->getBottomPoint();
    this->node->transformation->addVelocity(glm::vec3(0.0f,targetPos[1]-initialPos[1],0.0f));

    glUniform1i(glGetUniformLocation(programID_Entity,"numEntity"), numEntity);
    this->sendNodeToShader(this->node,programID_Entity,glm::mat4(1.0f));
    return damageFromEntity;
}

void Entity::setPave(Node* node, glm::vec3 dimensions, glm::vec3 position) {
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

void Entity::loadBufferNode(Node *node){
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

void Entity::sendNodeToShader(Node *node,GLuint programID_Entity,glm::mat4 parent){
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

void Entity::createZombie(Node* node, glm::vec3 position){
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
    this->setPave(chestZombie,glm::vec3(0.6,0.75,0.3), glm::vec3(0,0.82,0)+ position);
    this->setPave(headZombie,glm::vec3(0.5,0.5,0.5), glm::vec3(0,1.45,0)+ position);
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

void Entity::createCochon(Node* node, glm::vec3 position){
    Node *headCochon = new Node;
    Node *chestCochon = new Node;
    Node *leftArmCochon = new Node;
    Node *rightArmCochon = new Node;
    Node *leftLegCochon = new Node;
    Node *rightLegCochon = new Node;
    headCochon->nodeID=0;
    chestCochon->nodeID=1;
    leftArmCochon->nodeID=2;
    rightArmCochon->nodeID=3;
    leftLegCochon->nodeID=4;
    rightLegCochon->nodeID=5;
    setPave(chestCochon,glm::vec3(0.9,0.5,0.5), glm::vec3(0,0.2,0)+ position);
    setPave(headCochon,glm::vec3(0.45,0.45,0.45), glm::vec3(0.6,0.3,0)+ position);
    setPave(leftArmCochon,glm::vec3(0.25,0.4,0.25), glm::vec3(-0.32,-0.1,0.14)+ position);
    setPave(rightArmCochon,glm::vec3(0.25,0.4,0.25), glm::vec3(-0.32,-0.1,-0.14)+ position);
    setPave(leftLegCochon,glm::vec3(0.25,0.4,0.25), glm::vec3(0.4,-0.1,0.14)+ position);
    setPave(rightLegCochon,glm::vec3(0.25,0.4,0.25), glm::vec3(0.4,-0.1,-0.14)+ position);

    chestCochon->center=glm::vec3(0,0.8,0)+ position;
    headCochon->center=glm::vec3(0,1.5,0)+ position;
    rightArmCochon->center=glm::vec3(0.45,0.8,0)+ position;
    leftArmCochon->center=glm::vec3(0.15,0,0)+ position;
    rightLegCochon->center=glm::vec3(0.15,0,0)+ position;
    leftLegCochon->center=glm::vec3(-0.15,0,0)+ position;

    chestCochon->transformation = new Transform();
    headCochon->transformation = new Transform();
    rightArmCochon->transformation = new Transform();
    leftArmCochon->transformation = new Transform();
    rightLegCochon->transformation = new Transform();
    leftLegCochon->transformation = new Transform();

    node->fils.push_back(chestCochon);
    node->fils.push_back(headCochon);
    node->fils.push_back(leftArmCochon);
    node->fils.push_back(rightArmCochon);
    node->fils.push_back(leftLegCochon);
    node->fils.push_back(rightLegCochon);
}

void Entity::walkCochon(Node* node,float angle,float deltaTime){
    float angle1 = sin(angle);
    float angle2 = sin(angle+M_PI);

    glm::mat4 matTransfoArm1= glm::mat4(1.0f);
    glm::mat4 matTransfoArm2= glm::mat4(1.0f);
    glm::mat4 matTransfoLeg1= glm::mat4(1.0f);
    glm::mat4 matTransfoLeg2= glm::mat4(1.0f);
 
    matTransfoArm1 = glm::translate(matTransfoArm1 ,node->fils[4]->center+glm::vec3(-0.2f,0.1,0.0));
    matTransfoArm1 = glm::rotate(matTransfoArm1, angle1,glm::vec3(0.f,0.0f,1.0f));
    matTransfoArm1 = glm::translate(matTransfoArm1,-node->fils[4]->center-glm::vec3(-0.2f,0.1,0.0));

    matTransfoArm2 = glm::translate(glm::mat4(1.0f),node->fils[5]->center+glm::vec3(-0.46f,0.1,0.0));
    matTransfoArm2 = glm::rotate(matTransfoArm2, angle2,glm::vec3(0.f,0.0f,1.0f));
    matTransfoArm2 = glm::translate(matTransfoArm2,-node->fils[5]->center-glm::vec3(-0.48f,0.1,0.0));

    matTransfoLeg1 = glm::translate(glm::mat4(1.0f),node->fils[3]->center+glm::vec3(-0.02,-0.7,0.0));
    matTransfoLeg1 = glm::rotate(matTransfoLeg1, angle1,glm::vec3(0.f,0.0f,1.0f));
    matTransfoLeg1 = glm::translate(matTransfoLeg1,-node->fils[3]->center-glm::vec3(-0.02,-0.7,0.0));

    matTransfoLeg2 = glm::translate(glm::mat4(1.0f),node->fils[2]->center+glm::vec3(0.24,0.12f,0.24f));
    matTransfoLeg2 = glm::rotate(matTransfoLeg2, angle2,glm::vec3(0.f,0.0f,1.0f));
    matTransfoLeg2 = glm::translate(matTransfoLeg2,-node->fils[2]->center-glm::vec3(0.24,0.12f,0.24f));
    this->hitbox->move(this->agent->getDirection()*deltaTime*this->speedEntity);
    node->transformation->addVelocity(this->agent->getDirection()*deltaTime*this->speedEntity);
    node->fils[4]->transformation = new Transform(matTransfoLeg2);
    node->fils[5]->transformation = new Transform(matTransfoLeg1);
    node->fils[3]->transformation = new Transform(matTransfoArm2);
    node->fils[2]->transformation = new Transform(matTransfoArm1);
}

void Entity::walk(Node* node,float angle,float deltaTime){ // Le paramètre node doit être un zombie dans le graphe de scène
    float angleLeg1 = sin(angle);
    float angleLeg2 = sin(angle+M_PI);

    glm::mat4 matTransfoArm = node->fils[2]->transformation->getTransfoMat4();
    // Les 2 bras ont la même transformation
    matTransfoArm = glm::translate(glm::mat4(1.0f),node->fils[2]->center+glm::vec3(0.45,1.2,0));
    matTransfoArm = glm::rotate(matTransfoArm,(float)-M_PI/1.9f,glm::vec3(1.f,0.0f,0.0f));
    matTransfoArm = glm::translate(matTransfoArm,-node->fils[2]->center-glm::vec3(0.45,1.2,0));
    
    glm::mat4 matTransfoLeg1 = glm::translate(glm::mat4(1.0f),node->fils[4]->center+glm::vec3(0.15,0.4,0));
    matTransfoLeg1 = glm::rotate(matTransfoLeg1, angleLeg1,glm::vec3(1.f,0.0f,0.0f));
    matTransfoLeg1 = glm::translate(matTransfoLeg1,-node->fils[4]->center-glm::vec3(0.15,0.4,0));

    glm::mat4 matTransfoLeg2 = glm::translate(glm::mat4(1.0f),node->fils[5]->center+glm::vec3(0.15,0.4,0));
    matTransfoLeg2 = glm::rotate(matTransfoLeg2, angleLeg2,glm::vec3(1.f,0.0f,0.0f));
    matTransfoLeg2 = glm::translate(matTransfoLeg2,-node->fils[5]->center-glm::vec3(0.15,0.4,0));

    this->hitbox->move(this->agent->getDirection()*deltaTime*this->speedEntity);
    node->transformation->addVelocity(this->agent->getDirection()*deltaTime*this->speedEntity);
    node->fils[4]->transformation = new Transform(matTransfoLeg1);
    node->fils[5]->transformation = new Transform(matTransfoLeg2);
    node->fils[3]->transformation = new Transform(matTransfoArm);
    node->fils[2]->transformation = new Transform(matTransfoArm);
}

void Entity::reset(Node* node){ // Le paramètre node doit être un zombie dans le graphe de scène
 
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

void Entity::attack(Node* node, float deltaTime){ // Le paramètre node doit être un zombie dans le graphe de scène
    float accumulateurAnimation = this->agent->getAccumulateur();
    float angle = -M_PI/1.9; // Angle en radians (pour toutes les animations il vaudrait mieux que ce soit le cas pour ne pas confondre)

    // Les 2 bras ont la même transformation
    glm::mat4 matTransfoArm = glm::translate(glm::mat4(1.0f),node->fils[2]->center+glm::vec3(0.45,1.2,0));
    matTransfoArm = glm::rotate(matTransfoArm,angle,glm::vec3(1.f,0.0f,0.0f));
    matTransfoArm = glm::translate(matTransfoArm,-node->fils[2]->center-glm::vec3(0.45,1.2,0));
            
    node->fils[3]->transformation = new Transform(matTransfoArm);
    node->fils[2]->transformation = new Transform(matTransfoArm);
    
    if(accumulateurAnimation>0.8f){ // Ici, 0.8 c'est la durée de l'animation (sachant qu'on rajoute deltaTime à l'accumulateur, ce qui garantit l'indépendance entre temps d'animation et nombre de FPS)
        this->agent->resetAccumulateur();
        this->agent->setIsAttacking(false);
    }else{
        this->agent->setAccumulateur(this->agent->getAccumulateur()+deltaTime);
    }
}

void Entity::die(Node* node, bool *die, float *accumulateurAnimation, float deltaTime){ // Le paramètre node doit être un zombie dans le graphe de scène
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

void Entity::takeDamage(float damage){
    this->life -= damage;
}

float Entity::getLife(){
    return this->life;
}

void Entity::rotateEntity(float angleRotation){
    this->agent->addToAngleOfView(angleRotation);
    glm::mat4 matTransfoChest = this->node->transformation->getTransfoMat4();

    matTransfoChest = glm::translate(matTransfoChest,this->node->fils[1]->center);
    matTransfoChest = glm::rotate(matTransfoChest,angleRotation,glm::vec3(0.f,1.0f,0.0f));
    matTransfoChest = glm::translate(matTransfoChest,-this->node->fils[1]->center);

    this->node->transformation = new Transform(matTransfoChest);
}

int Entity::getType(){
    return this->type;
}

bool Entity::getNearPlayer(){
    return this->nearPlayer;
}

Node* Entity::getRootNode(){
    return this->node;
}
