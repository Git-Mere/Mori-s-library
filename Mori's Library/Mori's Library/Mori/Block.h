#pragma once
#include "../Engine/GameObject.h"

class Block : public GameObject {
public:
    Block(vec2 position, vec2 scale);
    GameObjectTypes Type() override { return GameObjectTypes::Block; }
    std::string TypeName() override { return "Block"; }
    void SetBlockPosition(vec2 position);
private:
    vec2 scale_size;
};