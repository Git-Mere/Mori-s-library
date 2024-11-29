#pragma once
#include "../Engine/GameState.h"

class Player;
class FontObject;

class Basement : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override { return "basement"; }
private:
    vec2 player_last_position = vec2(0, -350);
    vec2 player_last_velocity = vec2(0, 0);

    void initial_Wall();
    Player* player_ptr;
};