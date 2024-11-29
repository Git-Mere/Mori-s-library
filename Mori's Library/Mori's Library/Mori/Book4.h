#pragma once
#include "../Engine/GameState.h"

class Player;
class SkillUI;
class Fiter;
class GameClear;
class GameOver;
class PressAnyKey;
class Screen_Effect;

class Book4 : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Book4"; }

private:
    Player* player_ptr;
    SkillUI* skillui_ptr;
    Fiter* blossoming_blade_ptr;
    bool SoundFlag = false;

    GameOver* gameover;
    PressAnyKey* pressanykey;
    Screen_Effect* darkscreen;
    GameClear* gameclear;
    float dead_cooltime = 0;
    float dead_cooltime_max = 1;

    bool Sound_One_Time = false;
    bool stop_move_one_time = false;
    float background_sound;
};