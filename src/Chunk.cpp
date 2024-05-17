#include <Chunk.hpp>

#define CHUNK_SIZE 32

std::vector<std::vector<Structure>> Chunk::structures; // Permet d'éviter les erreurs de lien à la compilation

Chunk::Chunk(glm::vec3 position, int typeChunk, unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk, int seed){
    this->position = position;
    //this->ID=rand()%3;
    this->ID=0;
    if (typeChunk==0){
        this->buildFullChunk();
    }else if (typeChunk==1){
        this->buildSinusChunk();
    }else if (typeChunk==2){
        this->buildFlatChunk();
    }else if (typeChunk==3){
        this->buildProceduralChunk(dataPixels, widthHeightmap, heightHeightmap, posWidthChunk, posLengthChunk, seed);
    }
}

Chunk::Chunk(glm::vec3 position){
    this->position = position;
    this->buildEditorChunk();
}

Chunk::~Chunk(){
    std::cout << "Destruction de Chunk\n";
    for (int i = 0 ; i < this->listeVoxels.size() ; i++){
        delete this->listeVoxels[i];
    }
    glDeleteBuffers(1, &(this->vertexbuffer));
    glDeleteBuffers(1, &(this->elementbuffer));
}

void Chunk::buildEditorChunk(){
    this->listeVoxels.clear();
    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){ 
                if (i == CHUNK_SIZE/2 && j == CHUNK_SIZE/2 && k == CHUNK_SIZE/2){
                    Voxel *vox = new Voxel(glm::vec3(0.0,0.0,0.0),STONE_BLOCK); 
                    vox->setVisible(true);
                    this->listeVoxels.push_back(vox);
                }else{
                    this->listeVoxels.push_back(nullptr);
                }
            }
        }
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

void Chunk::buildProceduralChunk(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap, int posWidthChunk, int posLengthChunk, int seed){
    this->listeVoxels.clear();

    srand(seed+posWidthChunk+posLengthChunk);

    // Génération du terrain
    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){ 
                int indInText = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + i*4;
                if (k <= ((int)dataPixels[indInText])){ 
                    int typeBlock = rand() % 500;
                    int sizeVein = rand() % 3;
                    Voxel *vox;
                    if(this->ID==0){
                        vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),k>=(int)dataPixels[indInText]-(2+rand()%4) ? DIRT_BLOCK : (typeBlock==0?DIAMOND_ORE:(typeBlock<10?IRON_ORE:STONE_BLOCK))); 
                    }else if(this->ID==1){
                        vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),k>=(int)dataPixels[indInText]-(2+rand()%4) ? SAND_BLOCK : (typeBlock==0?DIAMOND_ORE:(typeBlock<10?IRON_ORE:STONE_BLOCK))); 
                    }else if(this->ID==2){
                        vox = new Voxel(glm::vec3(this->position[0]+i,this->position[1]+k,this->position[2]+j),k>=(int)dataPixels[indInText]-(2+rand()%4) ? SNOW_BLOCK : (typeBlock==0?DIAMOND_ORE:(typeBlock<10?IRON_ORE:STONE_BLOCK))); 
                    }
                    
                    if (k==(int)dataPixels[indInText]){
                        vox->setVisible(true);
                        if(this->ID==0){
                            vox->setId(GRASS_BLOCK);
                        }else if(this->ID==1){
                            vox->setId(SAND_BLOCK);
                        }else if(this->ID==2){
                            vox->setId(SNOW_BLOCK);
                        }
                    }else if (i*j*k==0 || i==CHUNK_SIZE-1 || j==CHUNK_SIZE-1){
                        vox->setVisible(true);
                    }
                    this->listeVoxels.push_back(vox);

                    // Génération des filons de minerais
                    int idToGenerate = listeVoxels[k*1024+32*j+i]->getID();
                    if((idToGenerate==DIAMOND_ORE || idToGenerate==IRON_ORE) && j>=sizeVein && i>=sizeVein && k>=sizeVein && j<CHUNK_SIZE){
                        for(int l=1;l<=sizeVein;l++){
                            for(int m=1;m<=sizeVein;m++){
                                for(int n=1;n<=sizeVein;n++){
                                    if(rand()%(m*4)==0)listeVoxels[k*1024+32*(j-m)+i]->setId(idToGenerate);
                                    if(rand()%(n*4)==0)listeVoxels[k*1024+32*j+(i-n)]->setId(idToGenerate);
                                    if(rand()%(n*4)==0)listeVoxels[k*1024+32*(j-m)+(i-n)]->setId(idToGenerate);
                                    if(rand()%(l*4)==0)listeVoxels[(k-l)*1024+32*j+i]->setId(idToGenerate);
                                    if(rand()%(m*4)==0)listeVoxels[(k-l)*1024+32*(j-m)+i]->setId(idToGenerate);
                                    if(rand()%(l*4)==0)listeVoxels[(k-l)*1024+32*j+(i-n)]->setId(idToGenerate);
                                    if(rand()%(m*4)==0)listeVoxels[(k-l)*1024+32*(j-m)+(i-n)]->setId(idToGenerate);
                                }
                            }
                        }
                    }
                }else{
                    this->listeVoxels.push_back(nullptr);
                }
            }
        }
    }

    // On place de la bedrock à la dernière couche
    for (int j=0;j<CHUNK_SIZE;j++){     
        for (int i=0;i<CHUNK_SIZE;i++){
            this->listeVoxels[j*CHUNK_SIZE+i]->setId(5);
        }
    }

    // On complète les trous de la génération (quand la différence de hauteur entre 2 blocs adjacents est > 1)
    for (int j=0;j<CHUNK_SIZE;j++){     
        for (int i=0;i<CHUNK_SIZE;i++){ 
            int indexVoxel = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + i*4;

            if(i<CHUNK_SIZE-1){
                int indexRightNeighbour = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + (i+1)*4;
                if(dataPixels[indexVoxel]-dataPixels[indexRightNeighbour]>1){
                    for(int k=dataPixels[indexVoxel]-1;k>dataPixels[indexRightNeighbour];k--){
                        listeVoxels[k*1024+32*j+i]->setVisible(true);
                    }
                }
            }
            if(i>0){
                int indexLeftNeighbour = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + (i-1)*4;
                if(dataPixels[indexVoxel]-dataPixels[indexLeftNeighbour]>1){
                    for(int k=dataPixels[indexVoxel]-1;k>dataPixels[indexLeftNeighbour];k--){
                        listeVoxels[k*1024+32*j+i]->setVisible(true);
                    }
                }
            }
            if(j<CHUNK_SIZE-1){
                int indexFrontNeighbour = posLengthChunk*4 + posWidthChunk*4 + (j+1)*widthHeightmap*4 + i*4;
                if(dataPixels[indexVoxel]-dataPixels[indexFrontNeighbour]>1){
                    for(int k=dataPixels[indexVoxel]-1;k>dataPixels[indexFrontNeighbour];k--){
                        listeVoxels[k*1024+32*j+i]->setVisible(true);
                    }
                }
            }
            if(j>0){
                int indexBackNeighbour = posLengthChunk*4 + posWidthChunk*4 + (j-1)*widthHeightmap*4 + i*4;
                if(dataPixels[indexVoxel]-dataPixels[indexBackNeighbour]>1){
                    for(int k=dataPixels[indexVoxel]-1;k>dataPixels[indexBackNeighbour];k--){
                        listeVoxels[k*1024+32*j+i]->setVisible(true);
                    }
                }
            }
        }
    }



    // On génère les structures (seulement après avoir généré le terrain)
    for (int k=0;k<CHUNK_SIZE;k++){
        for (int j=0;j<CHUNK_SIZE;j++){     
            for (int i=0;i<CHUNK_SIZE;i++){ 
                // On s'assure que la structure pourra rentrer dans le chunk
                int indInText = posLengthChunk*4 + posWidthChunk*4 + j*widthHeightmap*4 + i*4;
                if (k == ((int)dataPixels[indInText])+1 && i>1 && i<CHUNK_SIZE-2 && j>1 && j<CHUNK_SIZE-2 && rand()%100 == 0){ // Pour l'instant, les structures apparaissent aléatoirement
                    buildStructure(i,j,k);
                }
            }
        }
    }
}

Structure Chunk::readFile(std::ifstream &file){
    Structure resStructure;
    std::string line;

    while (std::getline(file, line)) { 
        std::istringstream flux(line);
        BlockStructure bs;
        for (int i = 0 ; i < 4 ; i++){
            flux >> bs.infoBlock[i];
        }
        resStructure.blocks.push_back(bs);
    }
    return resStructure;
}

void Chunk::setListeStructures(std::vector<std::vector<Structure>> liste){
    structures = liste;
}

void Chunk::buildStructure(int i, int j, int k){
    Structure to_build = structures[this->ID][rand()%structures[this->ID].size()]; // On construit l'une des structures disponibles 
    for (int n = 0 ; n < to_build.blocks.size() ; n++){
        
        int *infoBlock = to_build.blocks[n].infoBlock;
        if (!(i+infoBlock[1]<0||j+infoBlock[3]<0||k+infoBlock[2]<0||i+infoBlock[1]>=CHUNK_SIZE||j+infoBlock[3]>=CHUNK_SIZE||k+infoBlock[2]>=CHUNK_SIZE)){
            Voxel *actual_voxel = this->listeVoxels[(k + infoBlock[2])*1024 + ((j+infoBlock[3])%32) * 32 + ((i+infoBlock[1])%32)];
            if (actual_voxel != nullptr){ // Si lun voxel du terrain est généré à cet endroit, on le supprime pour le remplacer par celui de la structure
                delete actual_voxel;
            }
            Voxel *block = new Voxel(glm::vec3(this->position[0]+i+infoBlock[1],this->position[1]+k+infoBlock[2],this->position[2]+j+infoBlock[3]),infoBlock[0]); 
            block->setVisible(true);
            this->listeVoxels[(k + infoBlock[2])*1024 + ((j+infoBlock[3])%32) * 32 + ((i+infoBlock[1])%32)] = block;
        }
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
                listeVoxels[i]->setIdInChunk(compteur); // Les voxels auront besoin d'avoir connaissance de leurs positions dans les buffers (c'est pour l'animation de la destruction d'un bloc)
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
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &(this->vertices[0]), GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()* sizeof(unsigned int), &(this->indices[0]) , GL_DYNAMIC_DRAW);
}

void Chunk::drawChunk(){

    // Pour les ID des blocs, on utilise des shaders storage buffers
    glGenBuffers(1, &(this->shaderstoragebuffer));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->shaderstoragebuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->objectIDs.size()*sizeof(int), this->objectIDs.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->shaderstoragebuffer); // Attention : Dans le shader, binding doit valoir la même chose que le 2è paramètre
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

    glDeleteBuffers(1, &(this->shaderstoragebuffer)); // Attention à bien le supprimer, c'est ça qui causait la chute de FPS au bout d'un certain temps
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
