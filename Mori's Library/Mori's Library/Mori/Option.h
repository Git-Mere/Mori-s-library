#pragma once
#include "../Engine/GameState.h"
#include "Horazontal_Cursor.h"

inline float SOUND = 25;

class Horizontal_Cursor;

class Option : public GameState {
public:
    void Load() override;
    void Update([[maybe_unused]] float dt) override;
    void Unload() override;
    void Draw();
    std::string GetName() override { return "Option"; }
private:
    int select = 0;
    int total_select = 5;
    Horizontal_Cursor* cursor_ptr;
};