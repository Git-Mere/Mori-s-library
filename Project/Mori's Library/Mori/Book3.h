#pragma once
#include "../Engine/GameState.h"
#include "TextRead.h"

class Player;
class FontObject;
class Captain_attack;
class Captain;
class GameOver;
class PressAnyKey;
class GameClear;
class Screen_Effect;

class Book3 : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void Change_say();
    std::string GetName() override { return "Book3"; }

private:
    int previous_index = 0;
    Player* player_ptr;
    FontObject* say_object;
    Captain_attack* attack;
    Captain* captain;
    Sentence capatin_say;

    bool SoundFlag = false;
    GameOver* gameover;
    PressAnyKey* pressanykey;
    Screen_Effect* darkscreen;
    GameClear* gameclear;
    float dead_cooltime = 0;
    float dead_cooltime_max = 1;
    bool Sound_One_Time = false;
    float background_sound;
};