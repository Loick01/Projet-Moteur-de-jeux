#pragma once

#include "miniaudio.h"

#include <Headers.hpp>

class Sound{
    private :
        ma_engine engine;

        ma_sound* breakBlock;
        ma_sound* createBlock;
		ma_sound* playerDamage;
    public : 
        Sound();
        ~Sound();
        void playBreakSound();
        void playCreateSound();
        void playPlayerDamage();
        ma_engine* getRefToEngine();
};
