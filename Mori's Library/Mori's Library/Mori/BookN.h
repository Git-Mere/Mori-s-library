

#pragma once
#include "../Engine/GameState.h"
#include "TextRead.h"

class Player;
class FontObject;

class BookN : public GameState {
public:
    void Load() override;
    void Update(float dt) override;
    void Unload() override;
    void Draw() override;
    std::string GetName() override { return "BookN"; }

private:
    int previous_index = 0;
    Player* player_ptr;
    FontObject* say_object;
};