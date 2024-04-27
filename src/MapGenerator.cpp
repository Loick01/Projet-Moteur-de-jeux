#include <MapGenerator.hpp>

float MapGenerator::generatePerlinNoise(float x, float y, unsigned int seed){
  FastNoise noiseGenerator;
  //FastNoise::NoiseType nt = FastNoise::ValueFractal;
  //noiseGenerator.SetNoiseType(nt);
  //noiseGenerator.SetFractalOctaves(10);
  noiseGenerator.SetSeed(seed);
  return noiseGenerator.GetNoise(x,y);
}

MapGenerator::MapGenerator(int wMap, int hMap, unsigned int seed){
  this->widthMap = wMap;
  this->heightMap = hMap;
  this->seed = seed;
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
      float value = (((generatePerlinNoise(i/3,j/12, this->seed))+1)/2) * 31;
      dataPixels[i*widthHeightmap*4+j] = value;
    }
  }

  stbi_write_png("../Textures/terrain.png", widthHeightmap, heightHeightmap,4,dataPixels, 4*widthHeightmap);
}
