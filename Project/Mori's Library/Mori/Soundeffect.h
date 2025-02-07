#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <map>
#include <list>

enum SFX {
    //Player
    Parring,            
    Dash,      
    Death,     
    Player_heart,       
    Player_walk,
    Just_Parring,


    //Library
    LibraryBGM,        
    Book_open,          
    Book_close,

    //Slime
    Slime_attack,
    Slime_walking,
    Slime_jump,
    Slime_hurt,

    //Quiz
    Quiz_pop_out,       
    Quiz_timer,         
    Good_answer,        
    Bad_answer,         
    Blocking,           
    Earthquake,         
    One_falling,
    Quiz_hurt,

    //Captain
    Throwing,           

    //Fighter
    Charging,
    Energykick,
    Energypunch,
    Floor_punch,
    Punch_dash,
    Fighter_hit,
    Fighter_Real_hit,
    Energy_laser,

    //Robot
    Missile,
    lock_on,
    bomb,
    charging_laser,
    export_laser,
    Robot_hurt,

    //Angel
    Angel_laser,
    Angel_spear,
    Angel_teleport,
    Charging_ball,
    Angel_Hurt,

    //Devil
    Devil_laser,
    Devil_spear,
    Devil_teleport,
    Devil_charging_ball,
    Devil_Hurt,
    Devil_Floor_bomb,
    Devil_Floor_Sword,
    Devil_One_laser,

    //Background
    BibleBGM,          
    Book1BGM,           
    Book2BGM,           
    Book3BGM,           
    Book4BGM,           
    Book5BGM,          
    MainmenuBGM,        
    TutorialBGM,        
    FallenBibleBGM,
    ClearBGM,
    TOTAL_SOUNDS
};

class SoundEffect {
public:
    static void InitializeStaticSounds();
    static void PlaySoundEffect(SFX effect);
    static void StopSoundEffect(SFX effect);
    static void SetSoundEffectVolume(SFX effect, float volume);
    static void Loop_SoundEffect(SFX effect);
    static void Stop_Loop_SoundEffect(SFX effect);
    static void EraseSoundEffect();

private:
    sf::SoundBuffer buffer;
    static sf::SoundBuffer static_buffers[TOTAL_SOUNDS];
    static sf::Sound playing_sounds[TOTAL_SOUNDS];

    static const std::string soundFilePaths[TOTAL_SOUNDS];
};

#endif