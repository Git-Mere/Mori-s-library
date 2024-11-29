

#pragma once
#include "../Engine/GameState.h"
#include "Soundeffect.h"
class Cursor;

class Mainmenu : public GameState {
public:
    void Load() override;
    void Update([[maybe_unused]] float dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override { return "Mainmenu"; }
private:
    Cursor* cursor_ptr;
    int select = 0;
    int total_select = 4;
    bool first = false;
    bool SoundFlag = false;
};