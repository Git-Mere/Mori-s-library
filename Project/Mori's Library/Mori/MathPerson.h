#pragma once
#include "../Engine/GameObject.h"

class MathPerson : public GameObject {
public:
    MathPerson(vec2 position, vec2 scale);
    void Update([[maybe_unused]] float dt);
    GameObjectTypes Type() override { return GameObjectTypes::MathPerson; }
    std::string TypeName() override { return "MathPerson"; }
    void SetAnimation(bool animation);
    void SetPersonPosition(vec2 P_position);
private:
    enum class Animations {
        Idle,
        Angry
    };
    bool animation_state;
};