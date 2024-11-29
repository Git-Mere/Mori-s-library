#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"

class Furniture : public GameObject {
public:
    Furniture(vec2 position, vec2 scale, std::string sprite_name) : GameObject(position, 0, scale) {
		AddGOComponent(new Sprite("assets/sprite/Wall/" + sprite_name + ".spt", this));
    }

    GameObjectTypes Type() override { return GameObjectTypes::Furniture; }
    std::string TypeName() override { return "Furniture"; }

private:
};