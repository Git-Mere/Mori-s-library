#pragma once
#include "../Engine/GameState.h"
#include "../Mori/Soundeffect.h"

class Player;
class tuto_dummy;
class Mori;
class Mori_talk;

class Tutorial : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    void check_clear(float dt) override;
    void end_effect() override;
    std::string GetName() override { return "Tutorial"; }
private:
    Player* player_ptr;
    tuto_dummy* dummy_ptr;
    Mori* mori;
    Mori_talk* mori_talk;

    bool SoundFlag = false;
    float background_sound;

};