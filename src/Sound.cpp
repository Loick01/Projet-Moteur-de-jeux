#define MINIAUDIO_IMPLEMENTATION // Attention à ne pas oublier cette ligne, sinon miniaudio n'est pas correctement inclus
#include <Sound.hpp>

void Sound::playRandomBackground() {
    // Choisir un son aléatoirement
    int randomIndex = std::rand() % backgroundSoundList.size();
    ma_sound* randomSound = backgroundSoundList[randomIndex];

    ma_result result = ma_sound_start(randomSound);
    if (result != MA_SUCCESS) {
        std::cerr << "Erreur lors de la lecture de la musique avec le code d'erreur: " << result << std::endl;
    }
}

// Callback pour détecter la fin de la musique, et en lancer une nouvelle
void Sound::soundEndCallback(void* pUserData, ma_sound* pSound) {
    Sound* self = static_cast<Sound*>(pUserData);
    self->playRandomBackground();
}

Sound::Sound(){
    this->backgroundSoundList.clear();

    // Initialiser le moteur de miniaudio
    if (ma_engine_init(NULL, &(this->engine)) != MA_SUCCESS) {
        std::cerr << "Erreur lors de l'initialisation du moteur miniaudio" << std::endl;
    }

    std::vector<std::string> pathFileMusic = { // Ici on donne le chemin vers les musiques de fond que l'on souhaite utiliser
        "../Sound/BackgroundMusic/black1.mp3",
        "../Sound/BackgroundMusic/black2.mp3",
        "../Sound/BackgroundMusic/black3.mp3",
        "../Sound/BackgroundMusic/black4.mp3",
        "../Sound/BackgroundMusic/black5.mp3"
    };
    
    // Charger les fichiers de musique
    for (int i = 0 ; i < pathFileMusic.size() ; i++) {
        ma_sound* sound = new ma_sound; // Il faudra attention à la manière dont on nettoie la mémoire à la fin

        if (ma_sound_init_from_file(&(this->engine), pathFileMusic[i].c_str(), 0, NULL, NULL, sound) != MA_SUCCESS) {
            std::cerr << "Erreur lors de l'initialisation de la musique: " << pathFileMusic[i].c_str() << std::endl;
            delete sound;
            continue;
        }
        ma_sound_set_end_callback(sound, soundEndCallback, nullptr);
    
        this->backgroundSoundList.push_back(sound);
    }

    this->createBlock = new ma_sound;
    this->breakBlock = new ma_sound;
    ma_sound_init_from_file(&(this->engine), "../Sound/BlockEffect/breaking.mp3", 0, NULL, NULL, this->breakBlock); // On donne ici le chemin des fichiers audio pour les effets sonores
    ma_sound_init_from_file(&(this->engine), "../Sound/BlockEffect/placing.mp3", 0, NULL, NULL, this->createBlock);
}

Sound::~Sound(){
    ma_sound_uninit(this->breakBlock);
    delete this->breakBlock;
    ma_sound_uninit(this->createBlock);
    delete this->createBlock;

    for (int i = 0 ; i < this->backgroundSoundList.size() ; i++){
        ma_sound_uninit(this->backgroundSoundList[i]);
        delete this->backgroundSoundList[i];
    }
    ma_engine_uninit(&(this->engine)); // Attention à bien nettoyer la mémoire pour l'engine de miniaudio, sinon segfault
}

void Sound::playBreakSound(){
    ma_sound_start(this->breakBlock);
}

void Sound::playCreateSound(){
    ma_sound_start(this->createBlock);
}

