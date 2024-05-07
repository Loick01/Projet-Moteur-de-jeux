#pragma once

#include <Headers.hpp>

class Transform;

struct Node{
    std::vector<unsigned short> indices;
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<Node*> son;

    Transform *transformation;

    glm::vec3 centerNode;

    int ID;

    GLuint vbuffer;
    GLuint ebuffer;
};

class Zombie{
    private:
        Node *node;
        int hp;
        int att;
        int speed;
    public:
        Zombie(int ID,glm::vec3 pos);
        ~Zombie();
        void loadZombie();
        void drawZombie(GLuint programID_Entity);

        // A retirer d'ici
        void setPave(Node* node, glm::vec3 dimensions, glm::vec3 position);
        void loadBufferNode(Node *node);
        void sendNodeToBuffer(Node *node,GLuint programID_Entity,glm::mat4 parent);

        void createZombie(Node* node, glm::vec3 position);
        
        void walk(Node* node,float angle,float deltatime);
        void reset(Node* node);
        void attack(Node* node,bool *attack,int *time_Animation);
        void die(Node* node,bool *die,int *time_Animation);
            
        Node* getParentNode();
};