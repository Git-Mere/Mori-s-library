#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "Sphinx.h"
#include "Life.h"
#include "MagicBall.h"

extern float SOUND;

Sphinx::Sphinx(vec2 position, vec2 scale, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Book2/Sphinx.spt", this));
    SetScale(scale * 2);
    SetCollisionScale(scale, true);
    AddGOComponent(new Life(life));
    player_ = &player;
    hurt_timer = new DownTimer(0.0f);
    is_active = false;
    current_state->Enter(this);
}

void Sphinx::Update([[maybe_unused]] float dt)
{
    GameObject::Update(dt);
    playerPosition = player_->GetPosition();
    hurt_timer->Update(dt);

    if (hurt_timer->Remaining() == 0.0) {
        SetVelocity(vec2(0, 0));
        is_hurt = false;
    }
    if (GetGOComponent<Life>()->Dead()) {
        SetPosition(vec2(1000, 1000));
    }

    if (ontime == false && is_active == true) {
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Earthquake>>()->Emit(1, vec2(GetPosition().x, GetPosition().y), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
        ontime = true;
    }
    if (time > 0) {
        time -= dt;
        if (time <= 0) {
            hide = false;
        }
    }
}

void Sphinx::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
        if (!is_hurt && hide == false) {
            GetGOComponent<Life>()->Sub(1);
            SoundEffect::PlaySoundEffect(SFX::Quiz_hurt);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_hurt, SOUND);
            float x = (static_cast<Player*>(other_object)->GetPosition().x - GetPosition().x) * 2;
            float y = (static_cast<Player*>(other_object)->GetPosition().y - GetPosition().y) * 2;
            move_direction = { x , y };
            move_direction = normalize(move_direction);
            move_direction *= -50;
            SetVelocity(move_direction);
            hurt_timer->Set(hurt_time);
            is_hurt = true;
        }
        break;
    }
}

void Sphinx::Draw(const mat3& world_to_ndc){
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        if (hide == false) {
            GameObject::Draw(world_to_ndc);
        }
    }
}

int Sphinx::SphinxHelth() {
    return GetGOComponent<Life>()->GetLifeNumber();
}

void Sphinx::SetSphinxPosition(vec2 reposition) {
    SetPosition(reposition);
}

void Sphinx::Rest() {
    SetPosition(vec2(2000,2000));
    is_active = false;
}

void Sphinx::Active(vec2 reposition) {
    hide = true;
    time = max_time;
    if (is_active == false) {
        SetPosition(reposition);
    }
    is_active = true;
    ontime = false;
    pattern_count++;
}

int Sphinx::MaxLife() {
    return life;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
