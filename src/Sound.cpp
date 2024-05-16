#define MINIAUDIO_IMPLEMENTATION // Attention à ne pas oublier cette ligne, sinon miniaudio n'est pas correctement inclus
#include <Sound.hpp>

Sound::Sound(){
    // Initialiser le moteur de miniaudio
    if (ma_engine_init(NULL, &(this->engine)) != MA_SUCCESS) {
        std::cerr << "Erreur lors de l'initialisation du moteur miniaudio" << std::endl;
    }

    this->createBlock = new ma_sound;
    this->breakBlock = new ma_sound;
    this->playerDamage = new ma_sound;
    this->playerDie = new ma_sound;
    ma_sound_init_from_file(&(this->engine), "../Sound/BlockEffect/breaking.mp3", 0, NULL, NULL, this->breakBlock); // On donne ici le chemin des fichiers audio pour les effets sonores
    ma_sound_init_from_file(&(this->engine), "../Sound/BlockEffect/placing.mp3", 0, NULL, NULL, this->createBlock);
    ma_sound_init_from_file(&(this->engine), "../Sound/PlayerEffect/damage.mp3", 0, NULL, NULL, this->playerDamage);
    ma_sound_init_from_file(&(this->engine), "../Sound/PlayerEffect/playerDie.mp3", 0, NULL, NULL, this->playerDie);
}

Sound::~Sound(){
	std::cout << "Destructeur de Sound\n";
    ma_sound_uninit(this->breakBlock);
    delete this->breakBlock;
    ma_sound_uninit(this->createBlock);
    delete this->createBlock;
    ma_sound_uninit(this->playerDamage);
    delete this->playerDamage;
    ma_sound_uninit(this->playerDie);
    delete this->playerDie;

    ma_engine_uninit(&(this->engine)); // Attention à bien nettoyer la mémoire pour l'engine à la fin de miniaudio, sinon segfault
}

void Sound::playBreakSound(){
    ma_sound_start(this->breakBlock);
}

void Sound::playCreateSound(){
    ma_sound_start(this->createBlock);
}

void Sound::playPlayerDamage(){
    ma_sound_start(this->playerDamage);
}

void Sound::playDeadSound(){
    ma_sound_start(this->playerDie);
}


ma_engine* Sound::getRefToEngine(){
	return &(this->engine);
}