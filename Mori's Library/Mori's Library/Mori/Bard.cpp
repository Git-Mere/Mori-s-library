#include "../Engine/Particels.h"
#include "../Engine/Constant.h"
#include "Bard.h"
#include "Life.h"
#include "Cosmic_tone.h"
#include <random>
#include <iostream>

extern float SOUND;

Bard::Bard(vec2 position, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Book5/Robot/Robot.spt", this));
    SetScale(scale);
    hurt_timer = new DownTimer(0.0f);
    player_ = &player;
    current_state = &state_idle;
    current_state->Enter(this);
    SetCollisionScale(vec2{ ColSize }, true);

    if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        missile_speed = vec2{ 1200, 0 };
        missile_delay = 0.2f;
        laser_time = 0.7f;
        missile_speed2 = vec2{ 800, 0 };
        HP = 100;
    }
    AddGOComponent(new Life(HP));

     
}

void Bard::Update(float dt)
{
    //std::cout << GetGOComponent<Life>()->GetLifeNumber() << std::endl;
    playerPosition = player_->GetPosition();
    GameObject::Update(dt);
    hurt_timer->Update(dt);
    if (hurt_timer->Remaining() < 0.5) {
        stop_move = false;
    }
    if (hurt_timer->Remaining() == 0.0) {
        is_hurt = false;
    }
    if (GetGOComponent<Life>()->Dead()) {
        //if (is_animation_ended == true) {
        //    GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::teleport));
        //    is_animation_ended = false;
        //}
        //if (GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::teleport)) {
        //    if (GetGOComponent<Sprite>()->AnimationEnded()) {
        //        Destroy();
        //    }
        //}
        Destroy();
    }

    if (player_->Get_Life() <= 0) {
        change_state(&state_player_down);
    }

    if (has_summon == true)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-500.0f, 500.0f);
        for (int i = 0; i < 6; ++i)
        {
            float offsetX = dis(gen);
            float offsetY = dis(gen);

            vec2 summonPos = playerPosition + vec2{ offsetX, offsetY };

            summoner = new Summoner(summonPos, *player_);
            Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(summoner);
        }
        has_summon = false;
    }

    if (player_->get_damage() > 1)
    {
        player_->set_combo_time(dt);

        if (player_->get_combo_time() > 10)
        {
            player_->reset_combo_time();
            player_->reset_damage();
        }
    }
}

void Bard::Draw(const mat3& world_to_ndc)
{
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }
}

bool Bard::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    if (other_object_type == GameObjectTypes::parrying_attack)
        return true;
    return false;
}

void Bard::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
        if (!is_hurt) {
            stop_move = true;
            //float x = (static_cast<Player*>(other_object)->GetPosition().x - GetPosition().x) * 2;
            //float y = (static_cast<Player*>(other_object)->GetPosition().y - GetPosition().y) * 2;
            //move_direction = { x , y };
            //move_direction = normalize(move_direction);
            //move_direction *= -150;
            //SetVelocity(move_direction);
            GetGOComponent<Life>()->Sub(1);
            SoundEffect::PlaySoundEffect(SFX::Robot_hurt);
            SoundEffect::SetSoundEffectVolume(SFX::Robot_hurt, SOUND);
            hurt_timer->Set(hurt_time);
            is_hurt = true;
            player_->can_attack = false;
            player_->parrying_count = 0;
        }
        break;
    case GameObjectTypes::parrying_attack:
        if (!is_hurt) {
            stop_move = true;
            //float x = (static_cast<Player*>(other_object)->GetPosition().x - GetPosition().x) * 2;
            //float y = (static_cast<Player*>(other_object)->GetPosition().y - GetPosition().y) * 2;
            //move_direction = { x , y };
            //move_direction = normalize(move_direction);
            //move_direction *= -150;
            //SetVelocity(move_direction);
            GetGOComponent<Life>()->Sub(player_->get_damage());
            SoundEffect::PlaySoundEffect(SFX::Robot_hurt);
            SoundEffect::SetSoundEffectVolume(SFX::Robot_hurt, SOUND);
            hurt_timer->Set(hurt_time);
            is_hurt = true;
            player_->can_attack = false;
            player_->parrying_count = 0;
        }
        break;
    };
}

const vec2 Bard::Return_coordinate()
{
    return GetPosition();
}

void Bard::update_velocity(float dt) {
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

void Bard::State_Idle::Enter([[maybe_unused]] GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
}

void Bard::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void Bard::State_Idle::CheckExit(GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    if (bard->playerPosition.x > bard->GetPosition().x) {
        bard->change_state(&bard->state_running);
    }
    if (bard->playerPosition.x <= bard->GetPosition().x) {
        bard->change_state(&bard->state_running);
    }
    if (bard->playerPosition.y > bard->GetPosition().y) {
        bard->change_state(&bard->state_running);
    }
    if (bard->playerPosition.y <= bard->GetPosition().y) {
        bard->change_state(&bard->state_running);
    }
}

void Bard::State_Running::Enter([[maybe_unused]] GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    bard->attack_delay = 0;
    bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Running));
}

void Bard::State_Running::Update(GameObject* object, float dt) {
    Bard* bard = static_cast<Bard*>(object);
    if (!bard->GetGOComponent<Life>()->Dead()) {
        if (bard->playerPosition.x + 10 > bard->GetPosition().x) {
            bard->SetScale({ -bard->scale.x, bard->scale.y });
        }
        else if (bard->playerPosition.x - 10 <= bard->GetPosition().x) {
            bard->SetScale({ bard->scale.x, bard->scale.y });
        }
        else {
            bard->SetScale({ -bard->scale.x, bard->scale.y });
        }
        bard->update_velocity(dt);
        bard->attack_delay += dt;
    }
}

void Bard::State_Running::CheckExit([[maybe_unused]] GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    if (bard->attack_delay > 2) {
        bard->change_state(&bard->state_attack);
    }
}

void Bard::State_Attack::Enter([[maybe_unused]] GameObject* object)
{
    Bard* bard = static_cast<Bard*>(object);
    bard->SetVelocity({ 0,0 });
    bard->shooting_count = 0;
    bard->attack_delay = 0;
    bard->is_shoot = false;
    bard->is_pattern_finished = false;
    bard->angle = 0;
    bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Attack));
}

void Bard::State_Attack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Bard* bard = static_cast<Bard*>(object);
    //bard->playerPosition = bard->player_->GetPosition();
    bard->attack_delay += dt;
    vec2 bardPosition = bard->GetPosition();
    vec2 relativePosition = bard->playerPosition - bardPosition;
    bard->angle = atan2(relativePosition.y, relativePosition.x);

    if (bard->playerPosition.x + 10 > bard->GetPosition().x) {
        bard->SetScale({ -bard->scale.x, bard->scale.y });
    }
    else if (bard->playerPosition.x - 10 <= bard->GetPosition().x) {
        bard->SetScale({ bard->scale.x, bard->scale.y });
    }
    else {
        bard->SetScale({ -bard->scale.x, bard->scale.y });
    }
    while (bard->is_shoot == false && bard->attack_delay > bard->missile_delay) {

        bard->particle_vec2 = Math::RotationMatrix(bard->angle) * bard->missile_speed;

        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Cosmic_tone>>()->Rotate_Shot
        (1, vec2{ bard->GetPosition() }, vec2{ bard->particle_vec2 }, bard->angle - Math::PI/2);
        SoundEffect::PlaySoundEffect(SFX::Missile);
        SoundEffect::SetSoundEffectVolume(SFX::Missile, SOUND);
        bard->attack_delay = 0;
        bard->shooting_count++;

        if (bard->shooting_count >= 6) {
            bard->is_shoot = true;//
        }
    }

    bard->update_velocity(dt);
}

void Bard::State_Attack::CheckExit(GameObject* object)
{
    Bard* bard = static_cast<Bard*>(object);
    //
    //if (bard->attack_delay > 3)
    if(bard->is_shoot == true)
    {
        bard->change_state(&bard->state_summon);
    }
}

void Bard::State_summons::Enter([[maybe_unused]] GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    bard->SetVelocity({ 0,0 });
    bard->attack_delay = 0;
    bard->is_pattern_finished = false;
    bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Crime));
}

void Bard::State_summons::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Bard* bard = static_cast<Bard*>(object);
    bard->attack_delay += dt;
    if (bard->has_summon == false && bard->attack_delay > 5)
    {
        bard->has_summon = true;
        bard->is_pattern_finished = true;
    }
}

void Bard::State_summons::CheckExit(GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);

    if (bard->is_pattern_finished == true) {
        bard->change_state(&bard->state_crime);
    }
}

void Bard::State_crime::Enter([[maybe_unused]] GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    bard->SetVelocity({ 0,0 });
    bard->shooting_count = 0;
    bard->attack_delay = 0;
    bard->is_shoot = false;
    bard->is_pattern_finished = false;
    bard->angle = 0;
    bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Crime));
}

void Bard::State_crime::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Bard* bard = static_cast<Bard*>(object);
    bard->playerPosition = bard->player_->GetPosition();
    bard->attack_delay += dt;

    while (bard->is_shoot == false && bard->attack_delay > 3.0) {   
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
        for (int i = 0; i < 6; ++i)
        {
            float offsetX, offsetY;
            do {
                offsetX = dis(gen);
                offsetY = dis(gen);
            } while (offsetX >= -500.0f && offsetX <= 500.0f && offsetY >= -500.0f && offsetY <= 500.0f);

            vec2 summonPos = bard->playerPosition + vec2{ offsetX, offsetY };
            vec2 relativePosition = bard->playerPosition - summonPos;
            bard->angle = atan2(relativePosition.y, relativePosition.x);
            bard->particle_vec2 = Math::RotationMatrix(bard->angle) * bard->missile_speed2;

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Cosmic_tone>>()->Rotate_Shot
            (1, vec2{ summonPos }, vec2{ bard->particle_vec2 }, bard->angle - Math::PI / 2);
            SoundEffect::PlaySoundEffect(SFX::Missile);
            SoundEffect::SetSoundEffectVolume(SFX::Missile, SOUND);
        }

        bard->attack_delay = 0;
        bard->shooting_count++;
        bard->is_shoot = true;
        bard->is_pattern_finished = true;
    }

    //if (bard->attack_delay > 5)
    //{
    //    bard->is_pattern_finished = true;
    //}

}

void Bard::State_crime::CheckExit(GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);

    if (bard->is_pattern_finished == true) {
        bard->change_state(&bard->state_range);
    }
}


void Bard::State_range::Enter([[maybe_unused]] GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);
    bard->SetVelocity({ 0,0 });
    bard->attack_delay = 0;
    bard->is_pattern_finished = false;
    bard->time = 0;
    bard->randomX = rand() % 361 -270;
    //bard->x_position = bard->player_->GetPosition().x + bard->randomX;
    //bard->y_position = bard->player_->GetPosition().y;
    bard->draw_line = true;
    bard->is_shoot = false;
    bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Range));
    SoundEffect::PlaySoundEffect(SFX::charging_laser);
    SoundEffect::SetSoundEffectVolume(SFX::charging_laser, SOUND);

}

void Bard::State_range::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Bard* bard = static_cast<Bard*>(object);
    bard->attack_delay += dt;
    if (bard->draw_line && bard->attack_delay > 2)
    {
        if (bard->is_shoot == false)
        {
            bard->x_position = bard->player_->GetPosition().x + bard->randomX;
            bard->y_position = bard->player_->GetPosition().y;
            bard->is_shoot = true;
        }
        bard->time += dt;


        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Electronic_line>>()->Show
        (1, vec2{ static_cast<float>(bard->x_position),bard->y_position + 300 }, -Math::PI / 2);
         

        if (bard->time > bard->laser_time)
        {
            bard->time = 0;
            bard->draw_line = false;
            bard->draw_electronic = true;
            SoundEffect::PlaySoundEffect(SFX::export_laser);
            SoundEffect::SetSoundEffectVolume(SFX::export_laser, SOUND);
        }
    }

    if (bard->draw_electronic && bard->draw_line == false)
    {

        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Electronic>>()->Show
        (1, vec2{ static_cast<float>(bard->x_position),bard->y_position + 300 }, Math::PI / 2);

        bard->draw_electronic = false;
    }
    if (bard->attack_delay > 7)
    {
        bard->is_pattern_finished = true;
    }
}

void Bard::State_range::CheckExit(GameObject* object) {
    Bard* bard = static_cast<Bard*>(object);

    if (bard->is_pattern_finished == true) {
        bard->change_state(&bard->state_idle);
    }
}

//Player Down!!!!
void Bard::Player_Down::Enter([[maybe_unused]] GameObject* object)
{
    Bard* bard = static_cast<Bard*>(object);
    //bard->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    bard->stop_move = true;
    bard->SetVelocity(vec2(0, 0));
}

void Bard::Player_Down::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {}

void Bard::Player_Down::CheckExit([[maybe_unused]] GameObject* object) {}
