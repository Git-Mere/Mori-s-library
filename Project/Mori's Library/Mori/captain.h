#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"

class Captain : public GameObject {
public:
    Captain(vec2 position);
    //void Update([[maybe_unused]] float dt);
    //void Draw(const mat3& world_to_ndc);
    GameObjectTypes Type() override { return GameObjectTypes::Captain; }
    std::string TypeName() override { return "captain"; }
    void middle_attack(float speed);
    void parring(vec2 speed);
    void initialize();
private:

};

