#include <Chunk.hpp>

#define CHUNK_SIZE 32

Chunk::Chunk(glm::vec3 position, int typeChunk, unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk){
    this->position = position;
    if (typeChunk==0){
        this->buildFullChunk();
    }else if (typeChunk==1){
        this->buildSinusChunk();
    }else if (typeChunk==2){
        this->buildFlatChunk();
    }else if (typeChunk==3){
        this->buildProceduralChunk(dataPixels, widthHeightmap, heightHeightmap, posWidthChunk, posLengthChunk);
    }
}

void Chunk::buildFullChunk(){
    this->listeVoxels.clear();

    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){     
                Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),rand()%NB_BLOCK); 
                if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==CHUNK_SIZE-1){
                    vox->setVisible(true);
                    vox->setId(GRASS_BLOCK);
                }
                this->listeVoxels.push_back(vox);
            }
        }
    }
}

void Chunk::buildFlatChunk(){
    this->listeVoxels.clear();
    int hauteurMax = 3; // Nombre de couches générées
    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){     
                    if (k >= hauteurMax){
                        this->listeVoxels.push_back(nullptr);
                    }else{
                        Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),rand()%NB_BLOCK); 
                        if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==hauteurMax-1){
                            vox->setVisible(true);
                            vox->setId(GRASS_BLOCK);
                        }
                        this->listeVoxels.push_back(vox);
                    }
            }
        }
    }
}

// Avec ce modèle de génération, on voit qu'il y a un problème sur la détection de l'extérieur. Il faudra un algo pour déterminer les contours d'un chunk
void Chunk::buildSinusChunk(){
    this->listeVoxels.clear();

    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){
                float t = (float)i / CHUNK_SIZE-1;
                float s = (float)j / CHUNK_SIZE-1;
                float value = (std::sin(4 * M_PI * ((t+s)/2)) + 1.0f) * 0.5f; // Valeur entre 0 et 1
                //float value = (std::sin(2 * M_PI * t) + 1.0f) * 0.5f; // Valeur entre 0 et 1
                int heightVox = value*30 + 1; // Hauteur du cube entre 1 et 31
                if (k <= heightVox){
                    Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j), 0); 
                    if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==heightVox){
                        vox->setVisible(true);
                    }
                    this->listeVoxels.push_back(vox);
                }else{
                    this->listeVoxels.push_back(nullptr);
                }
            }
        }
    }
}

void Chunk::buildProceduralChunk(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk){
    this->listeVoxels.clear();

    srand (time(NULL)); // Là il faudrait mettre la seed, pour que l'aléatoire soit le même pour 2 seeds (pour l'instant on laisse comme ça)

    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){ 
                int indInText = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + i*4;
                if (k <= ((int)dataPixels[indInText])){ 
                    Voxel *vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),k%NB_BLOCK); 
                    if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1 ||k==((int)dataPixels[indInText])){
                        vox->setVisible(true);
                        vox->setId(GRASS_BLOCK);
                    }
                    this->listeVoxels.push_back(vox);
                }else{
                    this->listeVoxels.push_back(nullptr);
                }
            }
        }
    }

    // Pour l'instant c'est pas terrible on lit le fichier à chaque fois qu'on veut placer une structure, il faudrait conserver les infos pour les réutiliser

    //std::ifstream treeStructureFile("../Structures/Tree.txt");
    std::ifstream treeStructureFile("../Structures/Tree_2.txt");
    if (!treeStructureFile) {
        std::cerr << "Erreur lors de l'ouverture du fichier. Aucune structure construite" << std::endl;
        return;
    }

    // On génère les structures (seulement après avoir générer le terrain pour l'instant, à voir si il ne vaut pas mieux le faire en même temps que le terrain)
    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){ 
                // On s'assure que la structure pourra rentrer dans le chunk
                int indInText = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + i*4;
                if (k == ((int)dataPixels[indInText])+1 && k+5 <= 31 && rand()%100 == 0){ // Pour l'instant, les arbres apparaissent aléatoirement, mais sans utiliser la seed alors que c'est ce qu'on voudrait
                    Voxel *blockStructure = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),1); 
                    blockStructure->setVisible(true);
                    this->listeVoxels[k *1024 + (j%32) * 32 + (i%32)] = blockStructure;
                    buildStructure(treeStructureFile,i,j,k);
                    treeStructureFile.clear(); // Pour la prochaine fois, il faudra reprendre la lecture du fichier depuis le début
                    treeStructureFile.seekg(0);
                }
            }
        }
    }
    treeStructureFile.close();
}

void Chunk::buildStructure(std::ifstream &file, int i, int j, int k){
    std::string line;
    while (std::getline(file, line)) { 
        std::istringstream flux(line);

        int infoBlock[4];
        for (int i = 0 ; i < 4 ; i++){
            int v;
            flux >> v;
            infoBlock[i] = v;
        }

        Voxel *blockStructure = new Voxel(glm::vec3(this->position[0]+i+infoBlock[1],this->position[1]+k+infoBlock[2],this->position[2]+j+infoBlock[3]),infoBlock[0]); 
        blockStructure->setVisible(true);
        this->listeVoxels[(k + infoBlock[2])*1024 + ((j+infoBlock[3])%32) * 32 + ((i+infoBlock[1])%32)] = blockStructure;
    }
}

void Chunk::loadChunk(){
    // Très important de vider les vectors, sinon quand on modifie un chunk on ne voit aucune différence
    this->vertices.clear();
    this->indices.clear(); 
    int compteur = 0; // Nombre de voxel déjà chargé, pour savoir où en est le décalage des indices

    std::vector<int> objectIDs;

    for (int i = 0 ; i < this->listeVoxels.size() ; i++){
        if (listeVoxels[i] != nullptr){
            if (listeVoxels[i]->getVisible()){
                std::vector<glm::vec3> verticesVoxel = listeVoxels[i]->getVertices();
                this->vertices.insert(this->vertices.end(), verticesVoxel.begin(), verticesVoxel.end());

                for (int c = 0 ; c < 6 ; c++){ // Toutes les faces d'un voxel
                    unsigned int decalage = 24*compteur + c*4;
                    this->indices.push_back(decalage + 2);
                    this->indices.push_back(decalage + 0);
                    this->indices.push_back(decalage + 3);
                    this->indices.push_back(decalage + 3);
                    this->indices.push_back(decalage + 0);
                    this->indices.push_back(decalage + 1);
                }
                compteur++;

                objectIDs.push_back(listeVoxels[i]->getID());
            }
        }
    }

    this->objectIDs = objectIDs;
    
    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &(this->vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()* sizeof(unsigned int), &(this->indices[0]) , GL_STATIC_DRAW);
}

void Chunk::drawChunk(){

     // Pour les ID des blocs, on utilise des shaders storage buffers
    glGenBuffers(1, &(this->shaderstoragebuffer));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->shaderstoragebuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->objectIDs.size()*sizeof(int), this->objectIDs.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->shaderstoragebuffer); // Attention : Dans le shader binding doit valoir la même chose que le 2è paramètre
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    
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
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
}

std::vector<Voxel*> Chunk::getListeVoxels(){
    return this->listeVoxels;
}

void Chunk::setListeVoxels(std::vector<Voxel*> newListeVoxels){
    this->listeVoxels=newListeVoxels;
}

glm::vec3 Chunk::getPosition(){
    return this->position;
}