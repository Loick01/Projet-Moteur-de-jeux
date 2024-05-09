#include <TerrainControler.hpp>

TerrainControler::TerrainControler(int planeWidth, int planeLength, int planeHeight, int typeChunk, int seedTerrain, int octave){
    this->planeWidth = planeWidth;
    this->planeLength = planeLength; 
    this->planeHeight = planeHeight;
    this->typeChunk = typeChunk;
    this->seedTerrain = seedTerrain;
    this->octave = octave;

    // Chargement des structures
    std::vector<Structure> structures;
    std::ifstream tree_1_StructureFile("../Structures/Tree.txt");
    structures.push_back(Chunk::readFile(tree_1_StructureFile));
    std::ifstream tree_2_StructureFile("../Structures/Tree_2.txt");
    structures.push_back(Chunk::readFile(tree_2_StructureFile));
    std::ifstream houseStructureFile("../Structures/House_1.txt");
    structures.push_back(Chunk::readFile(houseStructureFile));
    Chunk::setListeStructures(structures);
    tree_1_StructureFile.close();
    tree_2_StructureFile.close();
    houseStructureFile.close();

    this->mg = new MapGenerator(this->planeWidth, this->planeLength, this->seedTerrain, this->octave); 
    this->mg->generateImage();
    int widthHeightmap, heightHeightmap, channels;
    unsigned char* dataPixels = stbi_load("../Textures/terrain.png", &widthHeightmap, &heightHeightmap, &channels, 4);
    this->buildPlanChunks(dataPixels, widthHeightmap, heightHeightmap);
    stbi_image_free(dataPixels);
}

TerrainControler::~TerrainControler(){
    for (int i = 0 ; i < this->listeChunks.size() ; i++){
        delete this->listeChunks[i];
    }
    delete mg;
}

std::vector<Chunk*> TerrainControler::getListeChunks(){
    return this->listeChunks;
}

void TerrainControler::buildPlanChunks(unsigned char* dataPixels, int widthHeightmap, int heightHeightmap){
    this->listeChunks.clear();
    for (int i = 0 ; i < this->planeWidth ; i++){
        for (int j = 0 ; j < this->planeLength ; j++){
            for (int k = 0 ; k < this->planeHeight ; k++){
                Chunk *c = new Chunk(glm::vec3((this->planeWidth*32)/2*(-1.f) + i*32,(this->planeHeight*32)/2*(-1.f) + k*32,(this->planeLength*32)/2*(-1.f) + j*32), this->typeChunk, dataPixels, widthHeightmap, heightHeightmap, i*32,j*32*this->planeWidth*32, seedTerrain); 
                c->loadChunk();
                this->listeChunks.push_back(c);
            }
        }
    }
}

int TerrainControler::getPlaneWidth(){
    return this->planeWidth;
}
int TerrainControler::getPlaneLength(){
    return this->planeLength;
}
int TerrainControler::getPlaneHeight(){
    return this->planeHeight;
}

LocalisationBlock TerrainControler::tryBreakBlock(glm::vec3 camera_target, glm::vec3 camera_position){
    glm::vec3 originPoint = camera_position;
    glm::vec3 direction = normalize(camera_target);
    for (int k = 1 ; k < RANGE+1 ; k++){ // Trouver une meilleure manière pour détecter le bloc à casser
    // for (float k = 0.1 ; k < RANGE+1. ; k+=0.1){
        glm::vec3 target = originPoint + (float)k*direction;
        int numLongueur = floor(target[0]) + 16*this->planeWidth;
        int numHauteur = floor(target[1]) + 16;
        int numProfondeur = floor(target[2]) + 16*this->planeLength;
        if (numLongueur < 0 || numLongueur > (this->planeWidth*32)-1 || numProfondeur < 0 || numProfondeur > (this->planeLength*32)-1 || numHauteur < 0 || numHauteur > 31){
            continue; // Attention à ne pas mettre return même si c'est tentant (par exemple si le joueur regarde vers le bas en étant au sommet d'un chunk)
        }else{
            int indiceV = numHauteur *1024 + (numProfondeur%32) * 32 + (numLongueur%32); // Indice du voxel que le joueur est en train de viser
            int indiceChunk = (numLongueur/32) * this->planeLength + numProfondeur/32;
            std::vector<Voxel*> listeVoxels = this->listeChunks[indiceChunk]->getListeVoxels();

            if (listeVoxels[indiceV] == nullptr){
                continue;
            }else{
                return {indiceV, indiceChunk, numLongueur, numProfondeur, numHauteur};
            }
        }
    }
    return {-1,-1,-1,-1,-1};
}

void TerrainControler::breakBlock(LocalisationBlock lb){ // Il faut déjà avoir testé (au minimum) si lb.indiceVoxel != -1 avant d'appeler cette fonction
    std::vector<Voxel*> listeVoxels = this->listeChunks[lb.indiceChunk]->getListeVoxels();

    delete listeVoxels[lb.indiceVoxel]; // Ne pas oublier de bien libérer la mémoire
    listeVoxels[lb.indiceVoxel] = nullptr;

    // Rendre visible les 6 cubes adjacents (s'ils existent et s'ils ne sont pas déjà visible)
    // Il faudrait chercher une meilleure façon de faire ça
    for (int c = -1 ; c < 2 ; c+=2){
        int numLongueurVoisin = (lb.numLongueur%32) + c;
        int numHauteurVoisin = lb.numHauteur + c;
        int numProfondeurVoisin = (lb.numProfondeur%32) + c;
        int indiceVoisin;

        if (numLongueurVoisin >= 0 && numLongueurVoisin <= 31){
            indiceVoisin = lb.numHauteur *1024 + (lb.numProfondeur%32) * 32 + numLongueurVoisin;
            if (listeVoxels[indiceVoisin] != nullptr && !(listeVoxels[indiceVoisin]->getVisible())){ // On vérifie si le voxel n'est pas déjà visible (en vrai c'est pas obligatoire)
                listeVoxels[indiceVoisin]->setVisible(true);
            }
        }
        if (numHauteurVoisin >= 0 && numHauteurVoisin <= 31){
            indiceVoisin = numHauteurVoisin *1024 + (lb.numProfondeur%32) * 32 + (lb.numLongueur%32);
            if (listeVoxels[indiceVoisin] != nullptr && !(listeVoxels[indiceVoisin]->getVisible())){
                listeVoxels[indiceVoisin]->setVisible(true);
            }
        }
        if (numProfondeurVoisin >= 0 && numProfondeurVoisin <= 31){
            indiceVoisin = lb.numHauteur *1024 + numProfondeurVoisin * 32 + (lb.numLongueur%32);
            if (listeVoxels[indiceVoisin] != nullptr && !(listeVoxels[indiceVoisin]->getVisible())){
                listeVoxels[indiceVoisin]->setVisible(true);
            }
        }
    }

    this->listeChunks[lb.indiceChunk]->setListeVoxels(listeVoxels);
    this->listeChunks[lb.indiceChunk]->loadChunk();
    //return;
}

void TerrainControler::tryCreateBlock(glm::vec3 camera_target, glm::vec3 camera_position, int typeBlock){
    glm::vec3 originPoint = camera_position;
    glm::vec3 direction = normalize(camera_target);
    float k = 3.0; // Pour l'instant le joueur ne peut poser un block qu'à cette distance
    glm::vec3 target = originPoint + (float)k*direction;
    int numLongueur = floor(target[0]) + 16*this->planeWidth;
    int numHauteur = floor(target[1]) + 16;
    int numProfondeur = floor(target[2]) + 16*this->planeLength;
    if (numLongueur < 0 || numLongueur > (this->planeWidth*32)-1 || numProfondeur < 0 || numProfondeur > (this->planeLength*32)-1 || numHauteur < 0 || numHauteur > 31){
        return;
    }else{
        int indiceV = numHauteur *1024 + (numProfondeur%32) * 32 + (numLongueur%32); // Indice du voxel que le joueur est en train de viser
        int indiceChunk = (numLongueur/32) * this->planeLength + numProfondeur/32;
        std::vector<Voxel*> listeVoxels = this->listeChunks[indiceChunk]->getListeVoxels();

        if (listeVoxels[indiceV] == nullptr){
            glm::vec3 posChunk = this->listeChunks[indiceChunk]->getPosition();
            Voxel* vox = new Voxel(glm::vec3(posChunk[0]+numLongueur%32,posChunk[1]+numHauteur,posChunk[2]+numProfondeur%32),typeBlock);
            vox->setVisible(true);
            listeVoxels[indiceV] = vox;

            // A voir si ici on ne pourrait pas mettre à jour la visibilité de certains voxels pour faire mieux

            this->listeChunks[indiceChunk]->setListeVoxels(listeVoxels);
            this->listeChunks[indiceChunk]->loadChunk();
        }
    }
}

void TerrainControler::drawTerrain(){
    for (int i = 0 ; i < this->listeChunks.size() ; i++){
        this->listeChunks[i]->drawChunk();
    }
}