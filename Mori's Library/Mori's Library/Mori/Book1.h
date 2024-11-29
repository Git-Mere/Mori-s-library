#pragma once
#include "../Engine/GameState.h"
class Player;
class Slime;
class GameClear;
class GameOver;
class PressAnyKey;
class Screen_Effect;

class Book1 : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Book1"; }
private:
    Player* player_ptr;
    Slime* slime_ptr;

    bool SoundFlag = false;
    GameClear* gameclear;
    GameOver* gameover;
    PressAnyKey* pressanykey;
    Screen_Effect* darkscreen;
    float dead_cooltime = 0;
    float dead_cooltime_max = 1;
    bool Sound_One_Time = false;
    bool stop_move_one_time = false;
    bool iput_one_time = false;
    float background_sound;
};