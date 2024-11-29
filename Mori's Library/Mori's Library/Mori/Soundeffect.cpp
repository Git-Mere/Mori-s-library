#include "soundeffect.h"

sf::SoundBuffer SoundEffect::static_buffers[TOTAL_SOUNDS];
sf::Sound SoundEffect::playing_sounds[TOTAL_SOUNDS];

const std::string SoundEffect::soundFilePaths[TOTAL_SOUNDS] = {
    //Player
    "assets/sounds/Player/parring.wav",
    "assets/sounds/Player/dash.wav",
    "assets/sounds/Player/death.wav",
    "assets/sounds/Player/player_heart.wav",
    "assets/sounds/Player/walking_player.wav",
    "assets/sounds/Player/Just_Parring.wav",  

    //Library
    "assets/sounds/Library/Library.wav",
    "assets/sounds/Library/book_open.wav",
    "assets/sounds/Library/book_close.wav",

    //Slime                  
    "assets/sounds/Slime/bubble.wav",
    "assets/sounds/Slime/Slime_walking.wav",
    "assets/sounds/Slime/Slime_jump.wav",
    "assets/sounds/Slime/Slime_hurt.wav",
    



    //Quiz
    "assets/sounds/Quiz/quiz_pop_out.wav",
    "assets/sounds/Quiz/quiz_timer.wav",
    "assets/sounds/Quiz/good_answer.wav",
    "assets/sounds/Quiz/bad_answer.wav",
    "assets/sounds/Quiz/blocking.wav",
    "assets/sounds/Quiz/earthquake.wav",
    "assets/sounds/Quiz/one_falling.wav",
    "assets/sounds/Quiz/Quiz_hurt.wav", 


    //Captain
    "assets/sounds/Captain/throwing.wav",

    //Fighter  //
    "assets/sounds/Fighter/charging.wav",
    "assets/sounds/Fighter/energyball.wav",
    "assets/sounds/Fighter/energypunch.wav",
    "assets/sounds/Fighter/floor_punch.wav",
    "assets/sounds/Fighter/punch_dash.wav",
    "assets/sounds/Fighter/fighter_hit.wav",      
    "assets/sounds/Fighter/Fighter_real_hit.wav", 
    "assets/sounds/Fighter/energy_laser.wav",


    //Robot    
    "assets/sounds/Robot/missile.wav",
    "assets/sounds/Robot/lock_on.wav", 
    "assets/sounds/Robot/bomb.wav",
    "assets/sounds/Robot/charging_laser.wav",
    "assets/sounds/Robot/laser.wav",
    "assets/sounds/Robot/Mechanic_Death.wav",  


    //Angel   
    "assets/sounds/Bible/Angel/angel_laser.wav", 
    "assets/sounds/Bible/Angel/angel_spear.wav",
    "assets/sounds/Bible/Angel/angel_teleport.wav",
    "assets/sounds/Bible/Angel/charging_ball.wav",
    "assets/sounds/Bible/Angel/Angel_hurt.wav",  

    //Devil  
    "assets/sounds/Bible/Devil/devil_laser.wav",  
    "assets/sounds/Bible/Devil/devil_spear.wav",
    "assets/sounds/Bible/Devil/devil_teleport.wav",
    "assets/sounds/Bible/Devil/devil_charging_ball.wav",  
    "assets/sounds/Bible/Devil/devil_hurt.wav",
    "assets/sounds/Bible/Devil/devil_floor.wav",     
    "assets/sounds/Bible/Devil/devil_swing.wav",  
    "assets/sounds/Bible/Devil/devil_one_laser.wav",
    

    //Background
    "assets/sounds/Backgrounds/Bible.wav",
    "assets/sounds/Backgrounds/Book1.wav",
    "assets/sounds/Backgrounds/Book2.wav",
    "assets/sounds/Backgrounds/Book3.wav",
    "assets/sounds/Backgrounds/Book4.wav",
    "assets/sounds/Backgrounds/Book5.wav",
    "assets/sounds/Backgrounds/Mainmenu.wav",
    "assets/sounds/Backgrounds/tutorial.wav",
    "assets/sounds/Backgrounds/FallenBible.wav",
    "assets/sounds/Backgrounds/clear_sound.wav", 

};

void SoundEffect::InitializeStaticSounds() {
    for (int i = 0; i < TOTAL_SOUNDS; ++i) {
        if (!static_buffers[i].loadFromFile(soundFilePaths[i])) {
            std::cout << "Failed to load sound file: " << soundFilePaths[i] << std::endl;
        }
    }
}

void SoundEffect::PlaySoundEffect(SFX effect) {
    playing_sounds[effect].setBuffer(static_buffers[effect]);
    playing_sounds[effect].play();
}

void SoundEffect::StopSoundEffect(SFX effect) {
    playing_sounds[effect].stop();
}

void SoundEffect::SetSoundEffectVolume(SFX effect, float volume) {
    playing_sounds[effect].setVolume(volume);
}

void SoundEffect::Loop_SoundEffect(SFX effect) {
    playing_sounds[effect].setLoop(true);
}

void SoundEffect::Stop_Loop_SoundEffect(SFX effect) {
    playing_sounds[effect].setLoop(false);
}

void SoundEffect::EraseSoundEffect() {
    for (int i = 0; i < TOTAL_SOUNDS; ++i) {
        playing_sounds[i].stop();
    }
}