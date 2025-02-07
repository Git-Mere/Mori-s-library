#include "Fiter.h"
#include "../Engine/Particels.h"
#include "Life.h"
#include <angles.h>
#include "FiterAttack.h"

extern float SOUND;

Fiter::Fiter(vec2 position, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Book4/Fiter/Fiter.spt", this));//
    SetScale(BladeSize);
    SetCollisionScale(vec2(BladeSize.x / 6, BladeSize.y / 4), true);
    hurt_timer = new DownTimer(0.0f);
    player_ = &player;
    current_state = &state_idle;
    current_state->Enter(this);
    if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        EnhanceHP = 3;
        Energy_kick_speed = vec2(400, 0);
        Energy_kick_delay = 0.3f;
        Energy_kick_count = 6;
        HP = 5;
    }
    AddGOComponent(new Life(HP));

}

void Fiter::Update(float dt)
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

    if (GetGOComponent<Life>()->Dead()) {
        Destroy();
    }

    if (player_->Get_Life() <= 0 ) {
        change_state(&state_player_down);
    }
}

void Fiter::Draw(const mat3& world_to_ndc)
{
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }
}

bool Fiter::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    return false;
}

void Fiter::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
        if (!is_hurt) {
            stop_move = true;
            is_hurt = true;
            hurt_timer->Set(hurt_time);
            SoundEffect::PlaySoundEffect(SFX::Fighter_hit);
            SoundEffect::SetSoundEffectVolume(SFX::Fighter_hit, SOUND);

            if (barrier == true) {
                BarrierHited();
                if (barrier_hited > (barrier_HP - 1)) {
                    barrier = false;
                }
            }
        }
        break;
    };
}

void Fiter::update_velocity(float dt) {
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

void Fiter::GetDamage() {
    GetGOComponent<Life>()->Sub(1);
    SoundEffect::PlaySoundEffect(SFX::Fighter_Real_hit);
    SoundEffect::SetSoundEffectVolume(SFX::Fighter_Real_hit, SOUND);
    barrier_hited = 0;
    barrier = true;
    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::BossHit>>()->Show(1, GetPosition(), 0.0f);
}

////////////////// animation ////////////////// 
void Fiter::State_Idle::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
}

void Fiter::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void Fiter::State_Idle::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->playerPosition.x > fiter->GetPosition().x) {
        fiter->change_state(&fiter->state_running);
    }
    if (fiter->playerPosition.x <= fiter->GetPosition().x) {
        fiter->change_state(&fiter->state_running);
    }
    if (fiter->playerPosition.y > fiter->GetPosition().y) {
        fiter->change_state(&fiter->state_running);
    }
    if (fiter->playerPosition.y <= fiter->GetPosition().y) {
        fiter->change_state(&fiter->state_running);
    }
}

//running
void Fiter::State_Running::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Running));
    fiter->attack_delay = 0;
}

void Fiter::State_Running::Update([[maybe_unused]] GameObject* object, float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (!fiter->GetGOComponent<Life>()->Dead()) {
        if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
            fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
        }
        else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
            fiter->SetScale({ -fiter->BladeSize.x, fiter->BladeSize.y });
        }
        else {
            fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
        }
        fiter->update_velocity(dt);
        fiter->attack_delay += dt;
    }
}

void Fiter::State_Running::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->attack_delay > 3) {
        fiter->change_state(&fiter->state_waitting);
    }
}

//Waiting
void Fiter::State_Waitting::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Step));
    fiter->attack_delay = 0;
    fiter->SetVelocity(vec2(0,0));
    if (fiter->GetGOComponent<Life>()->GetLifeNumber() > fiter->EnhanceHP) {
        delay_time = 2;
    }
    else {
        delay_time = 1.3;
    }
}

void Fiter::State_Waitting::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->attack_delay += dt;
}

void Fiter::State_Waitting::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->attack_delay > delay_time) {
        switch (pattern_number) {
        case 0:
            pattern_number++;
            fiter->change_state(&fiter->state_dash_attack);//state_dash_attack
            break;

        case 1:
            pattern_number++;
            fiter->change_state(&fiter->state_energy_kick);//state_energy_kick
            break;

        case 2:
            pattern_number++;
            fiter->change_state(&fiter->state_energy_fist);//state_energy_fist
            break;

        case 3:
            pattern_number++;
            fiter->change_state(&fiter->state_earthquake);//state_earthquake
            break;

        case 4:
            pattern_number = 0;
            fiter->change_state(&fiter->state_lazer);//state_lazer
            break;
        }
    }
}

//Dash attack
void Fiter::State_Dash_Attack::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->attack_delay = 0;
    fiter->PrePlayerPosition = fiter->player_->GetPosition();
    direction = normalize(fiter->PrePlayerPosition - fiter->GetPosition());
    ontime = false;
    animation_ontime = false;
    SoundEffect::PlaySoundEffect(SFX::Punch_dash);
    SoundEffect::SetSoundEffectVolume(SFX::Punch_dash, SOUND);

    if (fiter->GetGOComponent<Life>()->GetLifeNumber() > fiter->EnhanceHP) {
        Dash_Speed = 700;
        Dash_Time = 0.8;
    }
    else {
        Dash_Speed = 1000;
        Dash_Time = 0.7;
    }

    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash));
    if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
        fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
    }
    else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
        fiter->SetScale({ -fiter->BladeSize.x, fiter->BladeSize.y });
    }
    else {
        fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
    }
}

void Fiter::State_Dash_Attack::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->attack_delay <= Dash_Time) {
        fiter->SetVelocity(vec2(Dash_Speed * direction.x, direction.y * Dash_Speed));
        fiter->attack_delay += dt;
    }
    else if(ontime == false){
        fiter->SetVelocity(vec2(0, 0));
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Kick));
        ontime = true;
    }
}

void Fiter::State_Dash_Attack::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::Kick) && fiter->GetGOComponent<Sprite>()->AnimationEnded() && ontime == true) {
        fiter->SetVelocity(vec2(0,0));
        fiter->change_state(&fiter->state_idle);
    }
    if (fiter->screen_size.x < fiter->GetPosition().x || -fiter->screen_size.x > fiter->GetPosition().x ||
        fiter->screen_size.y < fiter->GetPosition().y || -fiter->screen_size.y > fiter->GetPosition().y )
    {
        fiter->SetVelocity(vec2(0, 0));
        fiter->change_state(&fiter->state_idle);
    }
}

//energy kick
void Fiter::State_Energy_Kick::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Pattern_Kick1));
    fiter->attack_delay = 0;
    onetime = false;
    count = 0;
    count_start = false;
}

void Fiter::State_Energy_Kick::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);

    if (count == 0 && fiter->GetGOComponent<Sprite>()->AnimationEnded() && onetime == false  ||
        fiter->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::Pattern_Kick1) && fiter->GetGOComponent<Sprite>()->AnimationEnded() && onetime == false) {
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
        onetime = true;

        vec2 relativePosition = normalize(fiter->playerPosition - fiter->GetPosition());
        fiter->angle = atan2(relativePosition.y, relativePosition.x);
        fiter->particle_vec2 = Math::RotationMatrix(fiter->angle) * fiter->Energy_kick_speed;
        if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Kick1>>()->Rotate_Shot(1, fiter->GetPosition(), vec2{ fiter->particle_vec2 }, fiter->angle);
            SoundEffect::PlaySoundEffect(SFX::Energykick);
            SoundEffect::SetSoundEffectVolume(SFX::Energykick, SOUND);
        }
        else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Kick2>>()->Rotate_Shot(1, fiter->GetPosition(), vec2{ fiter->particle_vec2 }, fiter->angle);
            SoundEffect::PlaySoundEffect(SFX::Energykick);
            SoundEffect::SetSoundEffectVolume(SFX::Energykick, SOUND);
        }
        else {
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Kick1>>()->Rotate_Shot(1, fiter->GetPosition(), vec2{ fiter->particle_vec2 }, fiter->angle);
            SoundEffect::PlaySoundEffect(SFX::Energykick);
            SoundEffect::SetSoundEffectVolume(SFX::Energykick, SOUND);
        }

        count++;

        if (count > fiter->Energy_kick_count) {
            fiter->pattern_finished = true;
        }
        fiter->attack_delay = 0.0f;
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Pattern_Kick2));
    }

    if (onetime == true && fiter->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::Pattern_Kick2) && fiter->GetGOComponent<Sprite>()->AnimationEnded()) {
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Step));
        count_start = true;
    }
    if (count_start == true) {
        fiter->attack_delay += dt;
    }

    if (fiter->attack_delay > fiter->Energy_kick_delay) {
        onetime = false;
        count_start = false;
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Pattern_Kick1));
    }
}

void Fiter::State_Energy_Kick::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->pattern_finished == true) {
        fiter->pattern_finished = false;
        fiter->attack_delay = 0.0f;
        fiter->change_state(&fiter->state_idle);
    }
}

//energy fist
void Fiter::State_Energy_Fist::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Punch));
    fiter->attack_delay = 0;
    fiter->is_created = false;
    onetime = false;
    create_count = 0;
}

void Fiter::State_Energy_Fist::Update(GameObject* object, float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (!fiter->GetGOComponent<Life>()->Dead()) {
        if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
            fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
        }
        else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
            fiter->SetScale({ -fiter->BladeSize.x, fiter->BladeSize.y });
        }
        else {
            fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
        }
    
        fiter->attack_delay += dt;
        vec2 relativePosition = normalize(fiter->playerPosition - fiter->GetPosition());
        fiter->angle = atan2(relativePosition.y, relativePosition.x);
        float timer = 0.2f;
        float initialAngle = fiter->angle - util::to_radians(30.0f);
        while (fiter->attack_delay >= timer && fiter->is_created == false) {
    
            fiter->particle_vec2 = Math::RotationMatrix(fiter->angle) * vec2 { 500, 0 };

            if (fiter->GetGOComponent<Life>()->GetLifeNumber() > fiter->EnhanceHP) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Fist>>()->Rotate_Shot
                (1, vec2{ fiter->GetPosition() }, vec2{ fiter->particle_vec2 }, fiter->angle);
                SoundEffect::PlaySoundEffect(SFX::Energypunch);
                SoundEffect::SetSoundEffectVolume(SFX::Energypunch, SOUND);
            }
            else {
                for (int i = 0; i < 3; ++i) {
                    float adjustedAngle = initialAngle + util::to_radians(30.0f * i);
                    fiter->particle_vec2 = Math::RotationMatrix(adjustedAngle) * vec2 { 500, 0 };
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Fist>>()->Rotate_Shot
                    (1, vec2{ fiter->GetPosition() }, vec2{ fiter->particle_vec2 }, adjustedAngle);
                    SoundEffect::PlaySoundEffect(SFX::Energypunch);
                    SoundEffect::SetSoundEffectVolume(SFX::Energypunch, SOUND);
                }
            }

            fiter->attack_delay = 0;
            create_count++;
    
            if (create_count >= 10) {
                fiter->is_created = true;
            }
        }
    
        if (2.0 >= fiter->attack_delay && fiter->attack_delay >= 1.5) {
            float orbitRadius = 750.0f;
            pre_angle = fiter->angle;
            orbitX = fiter->GetPosition().x + orbitRadius * cos(fiter->angle);
            orbitY = fiter->GetPosition().y + orbitRadius * sin(fiter->angle);
        }
    }
}

void Fiter::State_Energy_Fist::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->is_created) {
        fiter->pattern_finished = false;
        fiter->attack_delay = 0.0f;
        fiter->change_state(&fiter->state_idle);
    }
}

//Earthquake
void Fiter::State_Earthquake::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Earthquake));
    fiter->attack_delay = 0;
    fiter->is_collide = false;
    fiter->SetPosition(fiter->player_->GetPosition());
    SoundEffect::PlaySoundEffect(SFX::Floor_punch);
    SoundEffect::SetSoundEffectVolume(SFX::Floor_punch, SOUND);
    onetime = false;
    if (fiter->GetGOComponent<Life>()->GetLifeNumber() > fiter->EnhanceHP) {
        nermber = 3;
    }
    else {
        nermber = 5;
    }
    animation_check = false;
}

void Fiter::State_Earthquake::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (animation_check) {
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
        fiter->attack_delay += dt;
    }
    if (fiter->attack_delay > 1.6) {
        fiter->is_collide = true;
    }
    if (fiter->Fiter::GetGOComponent<Sprite>()->AnimationEnded() && !animation_check) {
        if (onetime == false) {
            animation_check = true;

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::PunchEarthquake>>()->Emit(1, fiter->GetPosition() + vec2(1, 1), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
            for (float i = 0; i < nermber; i++) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::PunchEarthquake>>()->Emit(1, fiter->GetPosition() + (topLeft * i), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::PunchEarthquake>>()->Emit(1, fiter->GetPosition() + (topRight * i), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::PunchEarthquake>>()->Emit(1, fiter->GetPosition() + (bottomLeft * i), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::PunchEarthquake>>()->Emit(1, fiter->GetPosition() + (bottomRight * i), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
            }

            onetime = true;
        }
    }
}

void Fiter::State_Earthquake::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->attack_delay > 3.3f) {
        fiter->attack_delay = 0.0f;
        onetime = false;
        fiter->change_state(&fiter->state_idle);
    }
}

//Energy lazer
void Fiter::State_Lazer::Enter([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Enerylazer1));
    fiter->attack_delay = 0;
    onetime = false;
    animation_check = true;
    timer_on = false;
    SoundEffect::PlaySoundEffect(SFX::Charging);
    SoundEffect::SetSoundEffectVolume(SFX::Charging, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::Charging);
}

void Fiter::State_Lazer::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (animation_check) {
        if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
            fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
        }
        else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
            fiter->SetScale({ -fiter->BladeSize.x, fiter->BladeSize.y });
        }
        else {
            fiter->SetScale({ fiter->BladeSize.x, fiter->BladeSize.y });
        }
    }

    if (fiter->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::Enerylazer1) && fiter->GetGOComponent<Sprite>()->AnimationEnded() && onetime == false) {
        animation_check = false;
        fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Enerylazer2));
        SoundEffect::StopSoundEffect(SFX::Charging);
        SoundEffect::Stop_Loop_SoundEffect(SFX::Charging);
        SoundEffect::PlaySoundEffect(SFX::Energy_laser);
        SoundEffect::SetSoundEffectVolume(SFX::Energy_laser, SOUND);
        SoundEffect::Loop_SoundEffect(SFX::Energy_laser);
        timer_on = true;
        if (fiter->GetGOComponent<Life>()->GetLifeNumber() > fiter->EnhanceHP) {
            if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Lazer_R>>()->Emit(1, vec2(fiter->GetPosition().x + 530, fiter->GetPosition().y), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);//+
            }
            else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Lazer_L>>()->Emit(1, vec2(fiter->GetPosition().x - 530, fiter->GetPosition().y), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);//-
            }
            else {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Energy_Lazer_R>>()->Emit(1, vec2(fiter->GetPosition().x + 530, fiter->GetPosition().y), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);//+
            }
        }
        else {///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////fix
            if (fiter->playerPosition.x + 10 > fiter->GetPosition().x) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::B_Energy_Lazer_R>>()->Rotate_Shot(1, vec2(fiter->GetPosition().x + 530, fiter->GetPosition().y), vec2(0.0f, 0.0f), 30.0f * lazer_angle);//+
            }
            else if (fiter->playerPosition.x - 10 <= fiter->GetPosition().x) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::B_Energy_Lazer_L>>()->Rotate_Shot(1, vec2(fiter->GetPosition().x - 530, fiter->GetPosition().y), vec2(0.0f, 0.0f), -30.0f * lazer_angle);//-
            }
            else {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::B_Energy_Lazer_R>>()->Rotate_Shot(1, vec2(fiter->GetPosition().x + 530, fiter->GetPosition().y), vec2(0.0f, 0.0f), 30.0f * lazer_angle);//+
            }
        }
        onetime = true;
    }
    if (timer_on) {
        fiter->attack_delay += dt;
    }
}

void Fiter::State_Lazer::CheckExit([[maybe_unused]] GameObject* object) {
    Fiter* fiter = static_cast<Fiter*>(object);
    if (fiter->attack_delay > 3) {
        fiter->attack_delay = 0.0f;
        onetime = false;
        SoundEffect::StopSoundEffect(SFX::Energy_laser);
        SoundEffect::Stop_Loop_SoundEffect(SFX::Energy_laser);
        fiter->change_state(&fiter->state_idle);
    }
}

//Player Down!!!!
void Fiter::Player_Down::Enter([[maybe_unused]] GameObject* object)
{
    Fiter* fiter = static_cast<Fiter*>(object);
    fiter->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Step));
    fiter->stop_move = true;
    fiter->SetVelocity(vec2(0, 0));
}

void Fiter::Player_Down::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {}

void Fiter::Player_Down::CheckExit([[maybe_unused]] GameObject* object) {}
