#include "Blessed_Aura.h"

Blessed_Aura::Blessed_Aura(vec2 position, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Bible/Blessed_Aura.spt", this));
    SetScale(Aurasize);
    player_ = &player;
    current_state = &invisible;
    current_state->Enter(this);
}
//
void Blessed_Aura::Draw(const mat3& world_to_ndc)
{
    GameObject::Draw(world_to_ndc);
}

void Blessed_Aura::State_Invisible::Enter([[maybe_unused]] GameObject* object) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    aura->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Invisible));
}

void Blessed_Aura::State_Invisible::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    aura->playerpos = aura->player_->GetPosition();
    aura->SetPosition({ aura->playerpos.x, aura->playerpos.y + 30 });
}

void Blessed_Aura::State_Invisible::CheckExit([[maybe_unused]] GameObject* object) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    if (aura->player_->parrying_count >= 3) {
        aura->change_state(&aura->visible);
    }
}

void Blessed_Aura::State_Visible::Enter([[maybe_unused]] GameObject* object) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    aura->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Visible));
}

void Blessed_Aura::State_Visible::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    aura->playerpos = aura->player_->GetPosition();
    aura->SetPosition({ aura->playerpos.x, aura->playerpos.y + 30 });

}

void Blessed_Aura::State_Visible::CheckExit([[maybe_unused]] GameObject* object) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    if (aura->player_->parrying_count >= 5) {
        aura->change_state(&aura->ultimate);
    }
}

void Blessed_Aura::State_Ultimate::Enter([[maybe_unused]] GameObject* object) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    aura->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ultimate));
}

void Blessed_Aura::State_Ultimate::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    aura->playerpos = aura->player_->GetPosition();
    aura->SetPosition({ aura->playerpos.x, aura->playerpos.y + 30 });
}

void Blessed_Aura::State_Ultimate::CheckExit([[maybe_unused]] GameObject* object) {
    Blessed_Aura* aura = static_cast<Blessed_Aura*>(object);
    if (aura->player_->parrying_count == 0) {
        aura->change_state(&aura->invisible);
    }
}