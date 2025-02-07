#include "../Engine/Particels.h"
#include "../Engine/Constant.h"
#include "Summoner.h"
#include "Life.h"
#include "Cosmic_tone.h"

extern float SOUND;
Summoner::Summoner(vec2 position, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Book5/HomingBomb.spt", this));
    SetScale(scale);
    hurt_timer = new DownTimer(0.0f);
    player_ = &player;
    current_state = &state_idle;
    current_state->Enter(this);
    SetCollisionScale(vec2{ ColSize }, true);
    AddGOComponent(new Life(1));//
}

void Summoner::Update(float dt)
{
    playerPosition = player_->GetPosition();
    GameObject::Update(dt);
    hurt_timer->Update(dt);
    if (hurt_timer->Remaining() < 0.5) {
        stop_move = false;
    }
    if (hurt_timer->Remaining() == 0.0) {
        is_hurt = false;
    }
    //if (GetGOComponent<Life>()->Dead()) {
    //    Destroy();
    //}
}

void Summoner::Draw(const mat3& world_to_ndc)
{
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }//
}

bool Summoner::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    if (other_object_type == GameObjectTypes::Player)
        return true;
    return false;
}

void Summoner::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
        is_boom = true;
        break;
    };
}

const vec2 Summoner::Return_coordinate()
{
    return GetPosition();
}

void Summoner::update_velocity(float dt) {
    float accel = acceleration + speed;
    float max_velo = max_velocity + speed;

    if (stop_move == false) {
        if (playerPosition.x > GetPosition().x) {
            UpdateVelocity({ accel * dt, 0 });
            if (GetVelocity().x < 0) {
                SetVelocity({ 0, GetVelocity().y });
            }
            if (GetVelocity().x > max_velo) {
                SetVelocity({ max_velo, GetVelocity().y });
            }
        }
        else if (playerPosition.x <= GetPosition().x) {
            UpdateVelocity({ -(accel * dt), 0 });
            if (GetVelocity().x > 0) {
                SetVelocity({ 0, GetVelocity().y });
            }
            if (GetVelocity().x < -max_velo) {
                SetVelocity({ -max_velo, GetVelocity().y });
            }
        }
        else {
            if (GetVelocity().x > drag * dt) {
                UpdateVelocity({ -(drag * dt), 0 });
            }
            else if (GetVelocity().x < -drag * dt) {
                UpdateVelocity({ drag * dt, 0 });
            }
            else {
                SetVelocity({ 0, GetVelocity().y });
            }
        }

        if (playerPosition.y > GetPosition().y) {
            UpdateVelocity({ 0, accel * dt });
            if (GetVelocity().y < 0) {
                SetVelocity({ GetVelocity().x, 0 });
            }
            if (GetVelocity().y > max_velo) {
                SetVelocity({ GetVelocity().x, max_velo });
            }
        }
        else if (playerPosition.y <= GetPosition().y) {
            UpdateVelocity({ 0, -(accel * dt) });
            if (GetVelocity().y > 0) {
                SetVelocity({ GetVelocity().x, 0 });
            }
            if (GetVelocity().y < -max_velo) {
                SetVelocity({ GetVelocity().x, -max_velo });
            }
        }
        else {
            if (GetVelocity().y > drag * dt) {
                UpdateVelocity({ 0, -(drag * dt) });
            }
            else if (GetVelocity().y < -drag * dt) {
                UpdateVelocity({ 0 , drag * dt });
            }
            else {
                SetVelocity({ GetVelocity().x, 0 });
            }
        }
    }
    else {
        SetVelocity({ 0, 0 });
    }
}

void Summoner::State_Idle::Enter([[maybe_unused]] GameObject* object) {
}

void Summoner::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void Summoner::State_Idle::CheckExit(GameObject* object) {
    Summoner* suicide_bot = static_cast<Summoner*>(object);
    if (suicide_bot->playerPosition.x > suicide_bot->GetPosition().x) {
        suicide_bot->change_state(&suicide_bot->state_running);
    }
    if (suicide_bot->playerPosition.x <= suicide_bot->GetPosition().x) {
        suicide_bot->change_state(&suicide_bot->state_running);
    }
    if (suicide_bot->playerPosition.y > suicide_bot->GetPosition().y) {
        suicide_bot->change_state(&suicide_bot->state_running);
    }
    if (suicide_bot->playerPosition.y <= suicide_bot->GetPosition().y) {
        suicide_bot->change_state(&suicide_bot->state_running);
    }
}

void Summoner::State_Running::Enter([[maybe_unused]] GameObject* object) {
    Summoner* suicide_bot = static_cast<Summoner*>(object);
    suicide_bot->attack_delay = 0;
    SoundEffect::PlaySoundEffect(SFX::lock_on);
    SoundEffect::SetSoundEffectVolume(SFX::lock_on, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::lock_on);
    
}

void Summoner::State_Running::Update(GameObject* object, float dt) {
    Summoner* suicide_bot = static_cast<Summoner*>(object);
    if (!suicide_bot->GetGOComponent<Life>()->Dead()) {
        if (suicide_bot->playerPosition.x + 10 > suicide_bot->GetPosition().x) {
            suicide_bot->SetScale({ suicide_bot->scale.x, suicide_bot->scale.y });
        }
        else if (suicide_bot->playerPosition.x - 10 <= suicide_bot->GetPosition().x) {
            suicide_bot->SetScale({ -suicide_bot->scale.x, suicide_bot->scale.y });
        }
        else {
            suicide_bot->SetScale({ suicide_bot->scale.x, suicide_bot->scale.y });
        }
        suicide_bot->update_velocity(dt);
        if (suicide_bot->is_boom)
        {
            suicide_bot->attack_delay += dt;
            suicide_bot->SetVelocity({ 0,0 });
        }

    }
}

void Summoner::State_Running::CheckExit([[maybe_unused]] GameObject* object) {
    Summoner* suicide_bot = static_cast<Summoner*>(object);
    if (suicide_bot->attack_delay > 0.5)
    {
        suicide_bot->change_state(&suicide_bot->state_attack);
        //���⼭ ���� ����
    }
}

void Summoner::State_Attack::Enter([[maybe_unused]] GameObject* object)
{
    Summoner* suicide_bot = static_cast<Summoner*>(object);
    suicide_bot->SetVelocity({ 0,0 });
    suicide_bot->attack_delay = 0;
    suicide_bot->is_pattern_finished = false;
    suicide_bot->is_boom = false;
    
    vec2 collisionPoint = suicide_bot->GetPosition();
Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Boom>>()
    ->Emit(1, collisionPoint, vec2{ 0,0 }, vec2{ 0,0 }, Math::PI);
    suicide_bot->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::explode));
}

void Summoner::State_Attack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Summoner* suicide_bot = static_cast<Summoner*>(object);
    suicide_bot->playerPosition = suicide_bot->player_->GetPosition();
    suicide_bot->attack_delay += dt;

    if (suicide_bot->playerPosition.x + 10 > suicide_bot->GetPosition().x) {
        suicide_bot->SetScale({ -suicide_bot->scale.x, suicide_bot->scale.y });
    }
    else if (suicide_bot->playerPosition.x - 10 <= suicide_bot->GetPosition().x) {
        suicide_bot->SetScale({ suicide_bot->scale.x, suicide_bot->scale.y });
    }
    else {
        suicide_bot->SetScale({ -suicide_bot->scale.x, suicide_bot->scale.y });
    }

    if (suicide_bot->attack_delay > 0.5)
    {
        SoundEffect::Stop_Loop_SoundEffect(SFX::lock_on);
        SoundEffect::PlaySoundEffect(SFX::bomb);
        SoundEffect::SetSoundEffectVolume(SFX::bomb, SOUND);

        suicide_bot->is_pattern_finished = true;
    }

    suicide_bot->update_velocity(dt);
}

void Summoner::State_Attack::CheckExit(GameObject* object)
{
    Summoner* suicide_bot = static_cast<Summoner*>(object);

    if (suicide_bot->is_pattern_finished == true) {
        suicide_bot->Destroy();
    }
}
