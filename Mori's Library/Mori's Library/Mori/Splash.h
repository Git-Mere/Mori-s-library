

#pragma once
#include "../Engine/GameState.h"

class Splash : public GameState {
public:
    Splash();
    void Load() override;
    void Update([[maybe_unused]] float dt) override;
    void Unload() override;
    void Draw() override;
    std::string GetName() override { return "Splash"; }
private:
    double counter = 0;
};
