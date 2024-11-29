#include "Angel_wings.h"

Angel_wings::Angel_wings(vec2 position, Lucifer& lucifer) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Bible/Angel_wings.spt", this));
    SetScale(WingsSize);
    lucifer_ = &lucifer;
    current_state = &visible;
    current_state->Enter(this);
}
//
void Angel_wings::Draw(const mat3& world_to_ndc)
{
    GameObject::Draw(world_to_ndc);
}

void Angel_wings::State_Visible::Enter([[maybe_unused]] GameObject* object) {
    Angel_wings* angel_wings = static_cast<Angel_wings*>(object);
    angel_wings->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Visible));
}

void Angel_wings::State_Visible::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Angel_wings* angel_wings = static_cast<Angel_wings*>(object);
    angel_wings->luciferPosition = angel_wings->lucifer_->GetPosition();
    angel_wings->SetPosition({angel_wings->luciferPosition.x, angel_wings->luciferPosition.y + 75});

}

void Angel_wings::State_Visible::CheckExit([[maybe_unused]]GameObject* object) {
    Angel_wings* angel_wings = static_cast<Angel_wings*>(object);
    if (!angel_wings->lucifer_->ReturnIsAnimationEnded()) {
        angel_wings->change_state(&angel_wings->invisible);
    }
}

void Angel_wings::State_Invisible::Enter([[maybe_unused]] GameObject* object) {
    Angel_wings* angel_wings = static_cast<Angel_wings*>(object);
    angel_wings->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Invisible));
}

void Angel_wings::State_Invisible::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Angel_wings* angel_wings = static_cast<Angel_wings*>(object);
    angel_wings->luciferPosition = angel_wings->lucifer_->GetPosition();
    angel_wings->SetPosition({ angel_wings->luciferPosition.x, angel_wings->luciferPosition.y + 75 });
}

void Angel_wings::State_Invisible::CheckExit([[maybe_unused]] GameObject* object) {
    Angel_wings* angel_wings = static_cast<Angel_wings*>(object);
    if (angel_wings->lucifer_->ReturnIsAnimationEnded()) {
        angel_wings->change_state(&angel_wings->visible);
    }
}