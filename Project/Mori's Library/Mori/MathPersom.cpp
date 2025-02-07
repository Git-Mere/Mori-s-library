#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "MathPerson.h"

MathPerson::MathPerson(vec2 position, vec2 scale): GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/Book2/MathPerson.spt", this));
    SetScale(scale);
    animation_state = true;
}

void MathPerson::Update([[maybe_unused]] float dt)
{
    if (animation_state) {
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    }
    else {
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Angry));
    }
}

void MathPerson::SetAnimation(bool animation) {
    animation_state = animation;
}

void MathPerson::SetPersonPosition(vec2 P_position) {
    SetPosition(P_position);
}