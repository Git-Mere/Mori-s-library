#include "../Engine/GameObjectManager.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Particels.h"
#include "../Engine/Constant.h"
#include "Slime.h"
#include "../ScreenEffect.h"

#include <iostream>

extern float SOUND;

Slime::Slime(Slime* parent, vec2 position, Player& player) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/Book1/Slime.spt", this));
    hurt_timer = new DownTimer(0.0f);
    attack_timer = new DownTimer(0.0f);
    current_state = &state_idle;
    current_state->Enter(this);
    player_ = &player;
    if (parent == nullptr)
    {
        SetVelocity(vec2(0, 0));
        size = static_cast<int>(default_size);
        if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
            size--;
        }
    }
    else {
        SetPosition(parent->GetPosition());
        size = parent->size - 1;
    }
    modeSetting();

    speed = default_speed[size];
    health = default_healths[size];
    scale = vec2{ default_scales[size] };
    SetScale(scale);
    SetCollisionScale(vec2{ scale * 0.4f }, true);
}

bool Slime::CanCollideWith(GameObjectTypes other_object_type) {
    if (this) {
        return false;
    }
    if (other_object_type == GameObjectTypes::Particle || other_object_type == GameObjectTypes::slime_ball || other_object_type == GameObjectTypes::SkillUI) {
        return false;
    }
    if (is_collide == false) {
        return false;
    }
    return true;
}

void Slime::Update(float dt)
{
    GameObject::Update(dt);
    hurt_timer->Update(dt);
    attack_timer->Update(dt);
    if (hurt_timer->Remaining() < 0.5) {
        stop_move = false;
    }
    if (hurt_timer->Remaining() == 0.0) {
        is_hurt = false;
    }
    if (Slime::GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Slime::Animations::Fade)) {
        if (Slime::GetGOComponent<Sprite>()->AnimationEnded()) {
            Destroy();
        }
    }

    if (player_->Get_Life() <= 0 && Slime::GetGOComponent<Sprite>()->CurrentAnimation() != static_cast<int>(Slime::Animations::Fade)) {
        change_state(&state_player_down);
    }
}

void Slime::Draw(const mat3& world_to_ndc)
{
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }
}

void Slime::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
    {
        if (!is_hurt) {
            stop_move = true;
            health -= 1;
            float x = (static_cast<Player*>(other_object)->GetPosition().x - GetPosition().x) * 2;
            float y = (static_cast<Player*>(other_object)->GetPosition().y - GetPosition().y) * 2;
            move_direction = { x , y };
            move_direction = normalize(move_direction);
            move_direction *= -150;
            SetVelocity(move_direction);
            SoundEffect::PlaySoundEffect(SFX::Slime_hurt);
            SoundEffect::SetSoundEffectVolume(SFX::Slime_hurt, SOUND);
            hurt_timer->Set(hurt_time);
            is_hurt = true;
        }
        break;
    }
    }
    if (health < 0)
    {
        RemoveGOComponent<Collision>();
        if (size > 0)
        {
            hurt_timer->Set(hurt_time);
            is_hurt = true;
            Slime* slime1 = new Slime(this, vec2(-2000, -2000), *player_);
            slime1->SetVelocity(Math::RotationMatrix((Math::PI / 2)) * GetVelocity() * 50);

            Slime* slime2 = new Slime(this, vec2(-2000, -2000), *player_);
            slime2->SetVelocity(Math::RotationMatrix(-(Math::PI / 2)) * GetVelocity() * 50);

            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(slime1);
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(slime2);
        }
    }
}

void Slime::modeSetting()
{
    if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        rotate_shot_count = 11;
        shot_speed *= 2;
    }
}

//다른곳에서 사용할 슬라임 좌표값
const vec2 Slime::Return_coordinate()
{
    return GetPosition();
}

bool Slime::Return_is_collide()
{
    return is_collide;
}

void Slime::update_velocity(float dt) {
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
}

void Slime::State_Idle::Enter([[maybe_unused]] GameObject* object){}

void Slime::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Slime* slime = static_cast<Slime*>(object);
    slime->playerPosition = slime->player_->GetPosition();
}

void Slime::State_Idle::CheckExit(GameObject* object) {
    Slime* slime = static_cast<Slime*>(object);
    if (slime->player_->Get_Life() <= 0) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
        slime->change_state(&slime->state_idle);
    }

    if (slime->playerPosition.x > slime->GetPosition().x) {
        slime->change_state(&slime->state_running);
    }
    if (slime->playerPosition.x <= slime->GetPosition().x) {
        slime->change_state(&slime->state_running);
    }
    if (slime->playerPosition.y > slime->GetPosition().y) {
        slime->change_state(&slime->state_running);
    }
    if (slime->playerPosition.y <= slime->GetPosition().y) {
        slime->change_state(&slime->state_running);
    }
    if (slime->health < 0) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Fade));
    }
}


void Slime::State_Running::Enter(GameObject* object)
{
    /*SoundEffect::PlaySoundEffect(SFX::Slime_jump);
    SoundEffect::SetSoundEffectVolume(SFX::Slime_jump, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::Slime_jump);*/
    Slime* slime = static_cast<Slime*>(object);//
    if (slime->Slime::GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Slime::Animations::randing_attack)) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    }
    if (slime->playerPosition.x > slime->GetPosition().x) {
        slime->SetScale({ -slime->scale.x, slime->scale.y });
    }
    else if (slime->playerPosition.x <= slime->GetPosition().x) {
        slime->SetScale({ slime->scale.x, slime->scale.y });
    }
}

void Slime::State_Running::Update(GameObject* object, float dt) {
    Slime* slime = static_cast<Slime*>(object);
    slime->long_range_attack_delay += dt;

    if (slime->playerPosition.x + 10 > slime->GetPosition().x) {
        slime->SetScale({ -slime->scale.x, slime->scale.y });
    }
    else if (slime->playerPosition.x - 10 <= slime->GetPosition().x) {
        slime->SetScale({ slime->scale.x, slime->scale.y });
    }
    else {
        slime->SetScale({ -slime->scale.x, slime->scale.y });
    }

    slime->playerPosition = slime->player_->GetPosition();
    slime->update_velocity(dt);//
    //slime->UpdatePosition({ slime->GetVelocity().x * dt, slime->GetVelocity().y * dt });
}

void Slime::State_Running::CheckExit(GameObject* object)
{
    Slime* slime = static_cast<Slime*>(object);
   

    if (slime->long_range_attack_delay > 3)
    {
        if (sqrt(pow(slime->playerPosition.x - slime->GetPosition().x, 2) + pow(slime->playerPosition.y - slime->GetPosition().y, 2)) < 300) {
            slime->change_state(&slime->state_long);
        }
        else {
            slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::jump_attack));
            //SoundEffect::StopSoundEffect(SFX::Slime_jump);
            //SoundEffect::Stop_Loop_SoundEffect(SFX::Slime_jump);
            slime->change_state(&slime->state_jump);
        }
        slime->long_range_attack_delay = 0;
    }
    if (slime->health < 0) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Fade));
        //SoundEffect::StopSoundEffect(SFX::Slime_jump);
        //SoundEffect::Stop_Loop_SoundEffect(SFX::Slime_jump);
    }
}


void Slime::State_Long_Range_Attack::Enter([[maybe_unused]] GameObject* object)
{
    Slime* slime = static_cast<Slime*>(object);
    slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Shoot));
    slime->theta = 0;
    slime->shooting_count = 0;
    slime->long_range_attack_delay = 0;
    slime->is_shoot = false;
    slime->is_pattern_finished = false;
    slime->attack_timer->Set(0.5f);
}

void Slime::State_Long_Range_Attack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Slime* slime = static_cast<Slime*>(object);
    slime->playerPosition = slime->player_->GetPosition();
    slime->long_range_attack_delay += dt;

    if (slime->playerPosition.x + 10 > slime->GetPosition().x) {
        slime->SetScale({ -slime->scale.x, slime->scale.y });
    }
    else if (slime->playerPosition.x - 10 <= slime->GetPosition().x) {
        slime->SetScale({ slime->scale.x, slime->scale.y });
    }
    else {
        slime->SetScale({ -slime->scale.x, slime->scale.y });
    }

    vec2 slimePosition = slime->GetPosition();
    vec2 relativePosition = slimePosition - slime->playerPosition;
    slime->angle = atan2(relativePosition.y, relativePosition.x);
    while (slime->is_shoot == false && slime->long_range_attack_delay > 0.2 && slime->attack_timer->Remaining() == 0) {
        slime->particle_vec2 = Math::RotationMatrix(slime->angle) * slime->shot_speed;

        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Slime_ball>>()->Rotate_Shot(
            1, vec2{ slime->GetPosition().x, slime->GetPosition().y - 15 }, slime->particle_vec2, slime->angle);
        SoundEffect::PlaySoundEffect(SFX::Slime_attack);
        SoundEffect::SetSoundEffectVolume(SFX::Slime_attack, SOUND);

        slime->long_range_attack_delay = 0;
        slime->shooting_count++;

        if (slime->shooting_count >= 3) {
            slime->is_shoot = true;
        }
    }

    if (slime->long_range_attack_delay > 1)
    {
        slime->is_pattern_finished = true;
    }
    slime->update_velocity(dt);
}

void Slime::State_Long_Range_Attack::CheckExit(GameObject* object)
{
    Slime* slime = static_cast<Slime*>(object);

    if (slime->is_pattern_finished == true) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
        slime->change_state(&slime->state_idle);
    }
    if (slime->health < 0) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Fade));
    }
}


void Slime::Jump_attack::Enter([[maybe_unused]] GameObject* object)
{
    Slime* slime = static_cast<Slime*>(object);
    slime->theta = 0;
    slime->shooting_count = 0;
    slime->long_range_attack_delay = 0;
    slime->current_slime_Yposition = slime->GetPosition().y;
    slime->is_shoot = false;
    slime->is_jump = false;
    slime->is_collide = false;
    slime->is_pattern_finished = false;
    slime->playerPosition = slime->player_->GetPosition();
}

void Slime::Jump_attack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Slime* slime = static_cast<Slime*>(object);
    slime->UpdatePosition({ 0 , slime->GetVelocity().y * dt });
    if (slime->is_jump == false) {
        //콜리젼 끔
        slime->is_collide = false;
        slime->SetVelocity({ 0, 1500 });
        if (slime->GetPosition().y > slime->current_slime_Yposition + 1000) {
            slime->is_jump = true;
        }
    }

    if (slime->is_jump == true) {
        slime->SetPosition({ slime->playerPosition.x, slime->GetPosition().y });
        if (slime->GetPosition().y > slime->playerPosition.y) {
            slime->SetVelocity({ 0, -1500 });
        }
        else {
            //콜리젼 킴
            if (slime->only_once == false) {
                Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(0.3f, 3, 5.f, 2);
                slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::randing_attack));
                SoundEffect::PlaySoundEffect(SFX::Slime_jump);
                SoundEffect::SetSoundEffectVolume(SFX::Slime_jump, SOUND);
                slime->only_once = !slime->only_once;
            }

            slime->is_collide = true;
            slime->long_range_attack_delay += dt;
            slime->SetVelocity({ 0, 0 });
            while (slime->is_shoot == false && slime->long_range_attack_delay > 0.05) {
                slime->particle_vec2 = Math::RotationMatrix(slime->theta) * slime->shot_speed;

                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Slime_ball>>()->Rotate_Shot(
                    1, vec2{ slime->GetPosition().x, slime->GetPosition().y }, slime->particle_vec2 , slime->theta);
                SoundEffect::PlaySoundEffect(SFX::Slime_attack);
                SoundEffect::SetSoundEffectVolume(SFX::Slime_attack, SOUND);
                slime->long_range_attack_delay = 0;
                //원하는각도 * PI/180;
                float rkreh =  360.f / slime->rotate_shot_count;
                slime->theta += rkreh * (Math::PI / 180);
                slime->shooting_count++;

                if (slime->shooting_count >= slime->rotate_shot_count) {
                    slime->is_shoot = true;
                    slime->is_pattern_finished = true;
                }
            }
        }
    }
}

void Slime::Jump_attack::CheckExit(GameObject* object)
{
    Slime* slime = static_cast<Slime*>(object);
    if (slime->is_pattern_finished) {
        slime->only_once = !slime->only_once;
        slime->change_state(&slime->state_idle);
    }
    if (slime->health < 0) {
        slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Fade));
    }
}

//Player Down!!!!
void Slime::Player_Down::Enter([[maybe_unused]] GameObject* object)
{
    Slime* slime = static_cast<Slime*>(object);
    slime->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    slime->stop_move = true;
    slime->SetVelocity(vec2(0, 0));
}

void Slime::Player_Down::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {}

void Slime::Player_Down::CheckExit([[maybe_unused]] GameObject* object) {}
