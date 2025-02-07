#pragma once
#include "../Engine/GameObject.h"

class ground : public GameObject {
public:
    ground(vec2 position, vec2 scale);
    GameObjectTypes Type() override { return GameObjectTypes::ground; }
    bool CanCollideWith(GameObjectTypes other_object_type);
    std::string TypeName() override { return "ground"; }
    void SetBlockPosition(vec2 position);
private:
    vec2 scale_size;
};