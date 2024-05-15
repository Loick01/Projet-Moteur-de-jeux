#pragma once

#include "miniaudio.h"

#include <Headers.hpp>

class Sound{
    private :
        ma_engine engine;
        std::vector<ma_sound*> backgroundSoundList;

        ma_sound* breakBlock;
        ma_sound* createBlock;

        static void soundEndCallback(void* pUserData, ma_sound* pSound);
    public : 
        Sound();
        ~Sound();
        void playBreakSound();
        void playCreateSound();
        void playRandomBackground();
};