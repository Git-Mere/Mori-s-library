#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H
#include "Soundeffect.h"
#include <vector>
#include <map>

enum BGM {

};

enum SFX {
    Jump_block,
    Parring,
    Dash,
    Grunt,
    Throw,
    Book_open,
    Book_close,
    Player_heart,
    Player_walk,
    Quiz_pop_out,
    Quiz_timer,
    Good_answer,
    Bad_answer,
	Blocking,
	Earthquake,
	One_falling,
	Blossom_walking

};

const map<SFX, SoundEffect*> sound_effects = {

    {
        SFX::Jump_block, new SoundEffect("assets/sounds/jump_block.wav")
    },
    {
        SFX::Parring, new SoundEffect("assets/sounds/parring.wav")
    },
    {
        SFX::Dash, new SoundEffect("assets/sounds/dash.wav")
    },
	{
		SFX::Grunt, new SoundEffect("assets/sounds/grunt.wav")
	},
	{
		SFX::Throw, new SoundEffect("assets/sounds/throwing.wav")
	},
	{
		SFX::Book_open, new SoundEffect("assets/sounds/book_open.wav")
	},
	{
		SFX::Book_close, new SoundEffect("assets/sounds/book_close.wav")
	},
	{
		SFX::Player_heart, new SoundEffect("assets/sounds/player_heart.wav")
	},
	{
		SFX::Player_walk, new SoundEffect("assets/sounds/walking_player.wav")
	},
	{
		SFX::Quiz_pop_out, new SoundEffect("assets/sounds/quiz_pop_out.wav")
	},
	{
		SFX::Quiz_timer, new SoundEffect("assets/sounds/quiz_timer.wav")
	},
	{
		SFX::Good_answer, new SoundEffect("assets/sounds/good_answer.wav")
	},
	{
		SFX::Bad_answer, new SoundEffect("assets/sounds/bad_answer.wav")
	},
	{
		SFX::Blocking, new SoundEffect("assets/sounds/blocking.wav")
	},
	{
		SFX::Earthquake, new SoundEffect("assets/sounds/earthquake.wav")
	},
	{
		SFX::One_falling, new SoundEffect("assets/sounds/one_falling.wav")
	},
	{
		SFX::Blossom_walking, new SoundEffect("assets/sounds/blossom_walking.wav")
	}

};

const map<BGM, SoundEffect*> backgrounds = {

};


void StopSoundEffect(SFX effect);
void PlaySoundEffect(SFX effect);
void SetSoundEffectVolume(SFX effect, float volume);
void Loop_SoundEffect(SFX effect);
void Stop_Loop_SoundEffect(SFX effect);
void EraseSoundEffect(SFX effect);
void EraseAll(); // have to fix


#endif