#pragma once
#include "../Engine/GameState.h"

class Credit : public GameState {
public:
    void Load() override;
    void Update([[maybe_unused]] float dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override { return "Credit"; }
private:
    int select = 0;
    int total_select = 4;
};