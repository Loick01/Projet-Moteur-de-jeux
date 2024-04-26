// blur.cpp : Floute une image en niveau de gris

#include "FastNoise.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION  // Cette ligne doit être définie avant d'inclure stb_image.h dans un seul fichier
#include "stb_image.h" 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

float generatePerlinNoise(float x, float y, unsigned seed){
  FastNoise noiseGenerator;
  noiseGenerator.SetSeed(seed);
  return noiseGenerator.GetNoise(x,y);
}

int main(int argc, char* argv[])
{
    char nom_fichier[250];
    int taille_map=0;


    sscanf (argv[1],"%s",nom_fichier) ;
    sscanf (argv[2],"%d",&taille_map) ;

  
  if (argc != 3 || taille_map%32!=0) {
    printf(" erreur : usage = nom_fichier entrée, taille map(%32)\n"); 
    exit (1) ;
  }
  //unsigned char* dataPixels = stbi_load("terrain3x3.png", &widthHeightmap, &heightHeightmap, &channels, 4);
  unsigned char* dataPixels=(unsigned char*)malloc(sizeof(unsigned char)*16*32*16*32*4);
  for(int i = 0 ; i < 16*32*16*32*4 ; i++){
    dataPixels[i] = 0;
    if(((i+1)%4)==0)dataPixels[i]=255;
  }
  int widthHeightmap=16*32;
  int heightHeightmap=16*32;

  unsigned int terrainSeed = 12384;
  

  for(int i=0;i<heightHeightmap;i++){
    for(int j=0;j<widthHeightmap*4;j+=4){

      float valeur=(((generatePerlinNoise(i/3,j/12, terrainSeed))+1)/2);
      if(valeur<0.2)valeur*=10;
      else valeur*=31;
    
      dataPixels[i*widthHeightmap*4+j]=valeur;
      //printf("valeur = %f\n",valeur);
    }
  }

  
  //printf("\n\n valeur = %f\n\n",noiseValue);


  stbi_write_png("terrain.png", widthHeightmap, heightHeightmap,4,dataPixels, 4*widthHeightmap);
   
   
    return 1;
}


