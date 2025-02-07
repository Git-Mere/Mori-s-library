#pragma once
#include "../Engine/GameState.h"

class Player;
class Lucifer;
class Angel_wings;
class Blessed_Aura;
class portalbook;
class GameOver;
class GameClear;
class PressAnyKey;
class Screen_Effect;

class Bible : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Bible"; }

private:
    Player* player_ptr;
    Lucifer* lucifer_ptr;
    Angel_wings* angel_wings_ptr;
    Blessed_Aura* aura_ptr;
    portalbook* the_end;

    bool SoundFlag = false;    
    GameOver* gameover;
    GameClear* gameclear;
    PressAnyKey* pressanykey;
    Screen_Effect* darkscreen;
    float dead_cooltime = 0;
    float dead_cooltime_max = 1;
    bool Sound_One_Time = false;
    bool stop_move_one_time = false;
    float background_sound;
};