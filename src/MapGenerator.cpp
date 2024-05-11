#include <MapGenerator.hpp>

float MapGenerator::generatePerlinNoise(float x, float y){
  FastNoise noiseGenerator;
  noiseGenerator.SetSeed(this->seed);
  float somme = noiseGenerator.GetNoise(x,y);
  for (int i = 1 ; i < this->octave ; i++){
    somme += noiseGenerator.GetNoise(x*2*i,y*2*i);
  }
  return somme/this->octave;
}

MapGenerator::MapGenerator(int wMap, int hMap, int seed, int octave){
  this->widthMap = wMap;
  this->heightMap = hMap;
  this->seed = seed;
  this->octave = octave;
}

MapGenerator::MapGenerator(){
  // Vide (il permet de régler le problème avec la supression du terrain en mode éditeur)
}

MapGenerator::~MapGenerator(){
  std::cout << "Destructeur de MapGenerator\n";
}

void MapGenerator::generateImage(){
    // A voir si on ne peut pas utiliser des images sur 3 canaux, voir en niveau de gris
    int dataSize = this->widthMap*32*this->heightMap*32*4;
    unsigned char* dataPixels=(unsigned char*)malloc(sizeof(unsigned char)*dataSize);
    for (int i = 0 ; i < dataSize ; i++){
        dataPixels[i] = 0;
        if(((i+1)%4)==0){
            dataPixels[i]=255;
        }
    }

    int widthHeightmap=this->widthMap*32;
    int heightHeightmap=this->heightMap*32;
  

  for(int i=0;i<heightHeightmap;i++){
    for(int j=0;j<widthHeightmap*4;j+=4){
      float value = ((generatePerlinNoise(i/3,j/12)+1)/2) * 31;
      dataPixels[i*widthHeightmap*4+j] = value;
    }
  }

  stbi_write_png("../Textures/terrain.png", widthHeightmap, heightHeightmap,4,dataPixels, 4*widthHeightmap);
  free(dataPixels);
}

void MapGenerator::setWidthMap(int widthMap){
  this->widthMap = widthMap;
}

void MapGenerator::setHeightMap(int heightMap){
  this->heightMap = heightMap;
}

void MapGenerator::setSeed(int seed){
  this->seed = seed;
}

void MapGenerator::setOctave(int octave){
  this->octave = octave;
}
