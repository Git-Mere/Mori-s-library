#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"

class Cursor : public GameObject {
public:
    Cursor(vec2 position, Math::rect range, int total);
    void Update([[maybe_unused]]float dt);
    //void Draw(const mat3& world_to_ndc);
    GameObjectTypes Type() override { return GameObjectTypes::Just_Image; }
    std::string TypeName() override { return "Cursor"; }
    void move_pos(int wh);
private:
    int total;
    int current_position;
    Math::rect range;
};

