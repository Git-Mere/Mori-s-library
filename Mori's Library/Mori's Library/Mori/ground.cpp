#include "ground.h"

ground::ground(vec2 position, vec2 scale = vec2(30)) : GameObject(position), scale_size(scale) {
    AddGOComponent(new Sprite("assets/sprite/ground.spt", this));//
    SetScale(scale_size);
}

bool ground::CanCollideWith(GameObjectTypes other_object_type)
{
    if (other_object_type == GameObjectTypes::Player) {
        return true;
    }
    return false;

}

void ground::SetBlockPosition(vec2 B_position) {
    SetPosition(B_position);
}