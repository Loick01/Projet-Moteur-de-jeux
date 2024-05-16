#pragma once

#include <Headers.hpp>

// Déclaration anticipée
class Transform; 
class TerrainControler;
class Agent;
class Player;

struct Node{
    std::vector<unsigned short> indices; // Indices des triangles concaténés dans une liste
    std::vector<glm::vec3> indexed_vertices;
    std::vector<Node*> fils; // Ensemble des fils de l'objet du noeud
    // Pas besoin de coordonnées uv, elles seront déterminées dans les shaders

    GLuint vertexbuffer;
    GLuint elementbuffer;

    int nodeID;
    Transform *transformation;
    glm::vec3 center;
};

class Entity{
    private:
        Node *node;
        float speedEntity;
        Hitbox *hitbox;
        Agent *agent;
        float life;
        int type; // 0 pour zombie, 1 pour cochon
        float vitesseRotationLeg;
        float vitesseRotation;
        float damageEntity;
        float areaDetection;
        bool nearPlayer;
    public:
        Entity(int type, int ID,glm::vec3 pos, float speedEntity,float entityHeight,float entityWidth, float entityLenght, float vitesseRotationLeg, float vitesseRotation, float damageEntity, float areaDetection);
        ~Entity();
        void loadEntity();
        float drawEntity(GLuint programID_Entity, int numEntity,float deltaTime,TerrainControler *terrainControler, Player *p);

        void loadBufferNode(Node *node);
        void sendNodeToShader(Node *node,GLuint programID_Entity,glm::mat4 parent);

        void setPave(Node* node, glm::vec3 dimensions, glm::vec3 position);

        void createZombie(Node* node, glm::vec3 position);
        void createCochon(Node* node, glm::vec3 position);

        void takeDamage(float damage);
        float getLife();
        
        void rotateEntity(float angleRotation);
        void walk(Node* node,float angle, float deltaTime);
        void walkCochon(Node* node,float angle,float deltaTime);
        void reset(Node* node);
        void attack(Node* node, float deltaTime);
        void die(Node* node, bool *die, float *accumulateurAnimation, float deltaTime);

        int getType();
        bool getNearPlayer();
            
        Node* getRootNode();
};