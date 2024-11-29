#pragma once
#include "../Engine/GameState.h"
#include "../Mori/FontObject.h"
class Player;
class tuto_dummy;

class Corridor : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Corridor"; }
private:
    Player* player_ptr;
    FontObject* announcement1;
    bool SoundFlag = false;
};