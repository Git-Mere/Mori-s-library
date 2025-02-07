///////////////////
///업그레이드 해야됨//
///////////////////
#include "Cursor.h"
#include "../Engine/Sprite.h"

Cursor::Cursor(vec2 position, Math::rect range, int total) : GameObject(position), range(range), total(total), current_position(0) {
    AddGOComponent(new Sprite("assets/sprite/Cursor.spt", this));
    SetScale(vec2(80));
}

void Cursor::Update([[maybe_unused]]float dt)
{
    float ran = range.Size().y / total;
    SetPosition(vec2(GetPosition().x, -ran * current_position));
}

//void Cursor::Draw(const mat3& world_to_ndc) {
//    GameObject::Draw(world_to_ndc);
//    vec2 tmp = GetScale();
//    SetScale(-tmp);
//    float tmp2 = GetPosition().x;
//    SetPosition(vec2(-tmp2, GetPosition().y));
//    GameObject::Draw(world_to_ndc);
//}

void Cursor::move_pos(int wh)
{
    current_position = wh;
}
