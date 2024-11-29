#pragma once
#include "../Engine/GameState.h"

class Player;
class Bard;
class GameOver;
class PressAnyKey;
class Screen_Effect;
class GameClear;

class Book5 : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Book5"; }

private:
    Player* player_ptr;
    Bard* bard_ptr;

    bool SoundFlag = false;
    GameOver* gameover;
    Screen_Effect* darkscreen;
    GameClear* gameclear;
    PressAnyKey* pressanykey;
    float dead_cooltime = 0;
    float dead_cooltime_max = 1;
    bool Sound_One_Time = false;
    bool stop_move_one_time = false;
    float background_sound;
};