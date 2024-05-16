#pragma once

#include "miniaudio.h"

#include <Headers.hpp>

class Sound{
    private :
        ma_engine engine;

        ma_sound* breakBlock;
        ma_sound* createBlock;
		ma_sound* playerDamage;
        ma_sound* playerDie;
    public : 
        Sound();
        ~Sound();
        void playBreakSound();
        void playCreateSound();
        void playPlayerDamage();
        void playDeadSound();
        ma_engine* getRefToEngine();
};
