#pragma once
#include "../Engine/GameState.h"

class Player;
class FontObject;
class portalbook;
class Mori;

class Library : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    

    std::string GetName() override { return "libarary"; }
private:
    void initial_Wall();
    vec2 player_last_position = vec2(-30, 200);
    vec2 player_last_velocity = vec2(0, 0);

    Mori* sit_mori;
    Player* player_ptr;
    portalbook* Basement_potal;
    float background_sound;
};