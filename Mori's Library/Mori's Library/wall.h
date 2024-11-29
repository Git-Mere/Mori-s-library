#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"

class Wall : public GameObject {
public:
    Wall(vec2 position, vec2 scale, std::string sprite_name, float rotate = 0);
    bool CanCollideWith(GameObjectTypes other_object_type);

    GameObjectTypes Type() override { return GameObjectTypes::Wall; }
    std::string TypeName() override { return "Wall"; }
    void Set_Collision(bool value);
private:
    bool collision_on = true;
};

