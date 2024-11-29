#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"


class Horizontal_Cursor : public GameObject {
public:
    Horizontal_Cursor(vec2 position, Math::rect range, int total);
    void Update(float dt) override;
    void move_pos(int wh);
    GameObjectTypes Type() override { return GameObjectTypes::Just_Image; }
    std::string TypeName() override { return "Horizontal_Cursor"; }

private:
    Math::rect range;
    int total;
    int current_position;
};
