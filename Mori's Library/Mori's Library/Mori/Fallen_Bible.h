#pragma once
#include "../Engine/GameState.h"

class Player;
class Fallen_Lucifer;
class Fallen_Angel_wings;
class Blessed_Aura;
class portalbook;
class GameOver;
class PressAnyKey;
class Screen_Effect;
class GameClear;

class Fallen_Bible : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Fallen_Bible"; }

private:
    Player* player_ptr;
    Fallen_Lucifer* fallen_lucifer_ptr;
    Fallen_Angel_wings* fallen_angel_wings_ptr;
    Blessed_Aura* aura_ptr;
    portalbook* the_end;

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