#include "Soundsource.h"

void PlaySoundEffect(SFX effect) {
    auto it = sound_effects.find(effect);
    if (it != sound_effects.end()) {
        SoundEffect* sound = it->second;
        sound->sound_play();
    }

}


void StopSoundEffect(SFX effect) {
    auto it = sound_effects.find(effect);
    if (it != sound_effects.end()) {
        SoundEffect* sound = it->second;
        sound->sound_stop();
    }

}

void SetSoundEffectVolume(SFX effect, float volume) {
    auto it = sound_effects.find(effect);
    if (it != sound_effects.end()) {
        SoundEffect* sound = it->second;
        sound->setvolume(volume);
    }

}

void Loop_SoundEffect(SFX effect) {
    auto it = sound_effects.find(effect);
    if (it != sound_effects.end()) {
        SoundEffect* sound = it->second;
        sound->set_loop(true);
    }

}

void Stop_Loop_SoundEffect(SFX effect) {
    auto it = sound_effects.find(effect);
    if (it != sound_effects.end()) {
        SoundEffect* sound = it->second;
        sound->set_loop(false);
    }

}

void EraseSoundEffect(SFX effect) {
    auto it = sound_effects.find(effect);
    if (it != sound_effects.end()) {
        SoundEffect* sound = it->second;
        sound->erase_one();

    }

}

void EraseAll() {
    for (auto it = sound_effects.begin(); it != sound_effects.end(); ++it) {
        SoundEffect* sound = it->second;
        sound->EraseAllSoundEffects();
    }
}
