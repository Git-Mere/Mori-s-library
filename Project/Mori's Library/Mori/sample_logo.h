#pragma once
#include "../Engine/GameObject.h"

class Logo : public GameObject {
public:
    Logo(vec2 position);
    GameObjectTypes Type() override { return GameObjectTypes::Just_Image; }
    std::string TypeName() override { return "Logo"; }
private:
};

