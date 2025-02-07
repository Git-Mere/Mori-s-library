#include "Horazontal_Cursor.h"
#include "../Engine/Sprite.h"

Horizontal_Cursor::Horizontal_Cursor(vec2 position, Math::rect range, int total)
    : GameObject(position), range(range), total(total), current_position(0) {
    AddGOComponent(new Sprite("assets/sprite/Horizontal_Cursor.spt", this));
    SetScale(vec2(80));
}

void Horizontal_Cursor::Update([[maybe_unused]] float dt) {
    float step = 180.0f;  
    SetPosition(vec2(range.Left() + step * current_position, GetPosition().y));
}

void Horizontal_Cursor::move_pos(int wh) {
    current_position = wh;
}
