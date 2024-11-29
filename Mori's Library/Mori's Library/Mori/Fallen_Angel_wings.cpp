#include "Fallen_Angel_wings.h"

Fallen_Angel_wings::Fallen_Angel_wings(vec2 position, Fallen_Lucifer& fallen_lucifer) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Bible/Fallen_Angel_wings.spt", this));
    SetScale(WingsSize);
    fallen_lucifer_ = &fallen_lucifer;
    current_state = &visible;
    current_state->Enter(this);
}

void Fallen_Angel_wings::Draw(const mat3& world_to_ndc)
{
    GameObject::Draw(world_to_ndc);
}

void Fallen_Angel_wings::State_Visible::Enter([[maybe_unused]] GameObject* object) {
    Fallen_Angel_wings* fallen_angel_wings = static_cast<Fallen_Angel_wings*>(object);
    fallen_angel_wings->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Visible));
}

void Fallen_Angel_wings::State_Visible::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fallen_Angel_wings* fallen_angel_wings = static_cast<Fallen_Angel_wings*>(object);
    fallen_angel_wings->luciferPosition = fallen_angel_wings->fallen_lucifer_->GetPosition();
    fallen_angel_wings->SetPosition({ fallen_angel_wings->luciferPosition.x, fallen_angel_wings->luciferPosition.y + 75 });

}

void Fallen_Angel_wings::State_Visible::CheckExit([[maybe_unused]] GameObject* object) {
    Fallen_Angel_wings* fallen_angel_wings = static_cast<Fallen_Angel_wings*>(object);
    if (!fallen_angel_wings->fallen_lucifer_->ReturnIsAnimationEnded()) {
        fallen_angel_wings->change_state(&fallen_angel_wings->invisible);
    }
}

void Fallen_Angel_wings::State_Invisible::Enter([[maybe_unused]] GameObject* object) {
    Fallen_Angel_wings* fallen_angel_wings = static_cast<Fallen_Angel_wings*>(object);
    fallen_angel_wings->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Invisible));
}

void Fallen_Angel_wings::State_Invisible::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fallen_Angel_wings* fallen_angel_wings = static_cast<Fallen_Angel_wings*>(object);
    fallen_angel_wings->luciferPosition = fallen_angel_wings->fallen_lucifer_->GetPosition();
    fallen_angel_wings->SetPosition({ fallen_angel_wings->luciferPosition.x, fallen_angel_wings->luciferPosition.y + 75 });
}

void Fallen_Angel_wings::State_Invisible::CheckExit([[maybe_unused]] GameObject* object) {
    Fallen_Angel_wings* fallen_angel_wings = static_cast<Fallen_Angel_wings*>(object);
    if (fallen_angel_wings->fallen_lucifer_->ReturnIsAnimationEnded()) {
        fallen_angel_wings->change_state(&fallen_angel_wings->visible);
    }
}