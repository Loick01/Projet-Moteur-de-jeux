#pragma once

#include <Headers.hpp>

class MapGenerator{
    private:
        int widthMap;
        int heightMap;
        unsigned int seed;
    public:
        MapGenerator(int wMap, int hMap, unsigned int seed);
        float generatePerlinNoise(float x, float y, unsigned int seed);
        void generateImage();
};