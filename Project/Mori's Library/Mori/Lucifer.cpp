#include "Lucifer.h"
#include "../Engine/Particels.h"
#include "../Engine/Constant.h"
#include "Final_Spark.h"
#include "Angel_wings.h"
#include "Light_spear.h"
#include "Javelin.h"
#include "Life.h"

extern float SOUND;

Lucifer::Lucifer(vec2 position, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Bible/Angel.spt", this));
    SetScale(scale);
    hurt_timer = new DownTimer(0.0f);
    player_ = &player;
    current_state = &state_idle;
    current_state->Enter(this);
    SetCollisionScale(vec2{ ColSize }, true);
    AddGOComponent(new Life(5));
    is_dead = false;
}

void Lucifer::Update(float dt)
{
    playerPosition = player_->GetPosition();
    GameObject::Update(dt);
    hurt_timer->Update(dt);
    if (hurt_timer->Remaining() < 0.5) {
        stop_move = false;
    }
    if (hurt_timer->Remaining() == 0.0) {
        if (current_state != &state_running) { SetVelocity(vec2(0, 0)); }
        is_hurt = false;
    }
    if (GetGOComponent<Life>()->Dead()) {
        is_dead = true;
        if (is_animation_ended == true) {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::teleport));
            is_animation_ended = false;
        }
        if (GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::teleport)) {
            if (GetGOComponent<Sprite>()->AnimationEnded()) {
                Destroy();
            }
        }
    }

    if (player_->Get_Life() <= 0 ) {
        change_state(&state_player_down);
    }
}

bool Lucifer::LuciferDead() {
    return is_dead;
}

void Lucifer::Draw(const mat3& world_to_ndc)
{
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }
}

bool Lucifer::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    return false;
}

void Lucifer::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
        if (!is_hurt) {
            stop_move = true;
            float x = (static_cast<Player*>(other_object)->GetPosition().x - GetPosition().x) * 2;
            float y = (static_cast<Player*>(other_object)->GetPosition().y - GetPosition().y) * 2;
            move_direction = { x , y };
            move_direction = normalize(move_direction);
            move_direction *= -150;
            SetVelocity(move_direction);
            GetGOComponent<Life>()-> Sub(1);
            SoundEffect::PlaySoundEffect(SFX::Angel_Hurt);
            SoundEffect::SetSoundEffectVolume(SFX::Angel_Hurt, SOUND);
            hurt_timer->Set(hurt_time);
            is_hurt = true;
            player_->can_attack = false;
            player_->parrying_count = 0;
        }
        break;
    };
}

const vec2 Lucifer::Return_coordinate()
{
    return GetPosition();
}

void Lucifer::update_velocity(float dt) {
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

vec2 Lucifer::teleport_place()
{
    vec2 col = GetCollisionScale();
    const vec2 camera_position = Engine::GetGameStateManager().GetGSComponent<CameraComponent>()->GetPosition();
    const vec2 half_window_size = Engine::Instance()->Get_Window() / 2;
    vec2 left = { playerPosition.x - (Distance2),  playerPosition.y };
    vec2 down = { playerPosition.x,  playerPosition.y - (Distance2) };
    vec2 right = { playerPosition.x + (Distance2),  playerPosition.y };
    vec2 up = { playerPosition.x ,  playerPosition.y + (Distance2) };

    const float left_limit = ((camera_position.x - half_window_size.x) + (col.x / 2));
    const float right_limit = ((camera_position.x + half_window_size.x) - (col.x / 2));
    const float bottom_limit = ((camera_position.y - half_window_size.y) + (col.y / 2));
    const float top_limit = ((camera_position.y + half_window_size.y) - (col.y / 2));

    if (left.x > left_limit) {
        appear_position.push_back(left);
    }
    if (right.x < right_limit) {
        appear_position.push_back(right);
    }
    if (down.y > bottom_limit) {
        appear_position.push_back(down);
    }
    if (up.y < top_limit) {
        appear_position.push_back(up);
    }

    util::using_random_seed();
    int rannum = util::random(0, static_cast<int>(appear_position.size()));

    return appear_position[rannum];
}

void Lucifer::State_Idle::Enter([[maybe_unused]] GameObject* object) {
}

void Lucifer::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void Lucifer::State_Idle::CheckExit(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (lucifer->playerPosition.x > lucifer->GetPosition().x) {
        lucifer->change_state(&lucifer->state_running);
    }
    if (lucifer->playerPosition.x <= lucifer->GetPosition().x) {
        lucifer->change_state(&lucifer->state_running);
    }
    if (lucifer->playerPosition.y > lucifer->GetPosition().y) {
        lucifer->change_state(&lucifer->state_running);
    }
    if (lucifer->playerPosition.y <= lucifer->GetPosition().y) {
        lucifer->change_state(&lucifer->state_running);
    }
}

void Lucifer::State_Running::Enter(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->attack_delay = 0;
}

void Lucifer::State_Running::Update(GameObject* object, float dt) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        if (lucifer->playerPosition.x + 10 > lucifer->GetPosition().x) {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        else if (lucifer->playerPosition.x - 10 <= lucifer->GetPosition().x) {
            lucifer->SetScale({ -lucifer->scale.x, lucifer->scale.y });
        }
        else {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        lucifer->update_velocity(dt);
        lucifer->attack_delay += dt;
    }
}

void Lucifer::State_Running::CheckExit(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    //lucifer->change_state(&lucifer->holy_sword);
    if (lucifer->attack_delay > 3) {
        lucifer->change_state(&lucifer->charging_light);
    }
}

void Lucifer::Charging_light::Enter(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->charge_delay = 0;
    lucifer->create_count = 0;
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_charged = false;
    SoundEffect::PlaySoundEffect(SFX::Charging_ball);
    SoundEffect::SetSoundEffectVolume(SFX::Charging_ball, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::Charging_ball);
    lucifer->current_pos = { lucifer->GetPosition() };
    lucifer->light_position[0] = { lucifer->current_pos.x - lucifer->Distance,  lucifer->current_pos.y - lucifer->Distance };
    lucifer->light_position[1] = { lucifer->current_pos.x - lucifer->Distance,  lucifer->current_pos.y + lucifer->Distance };
    lucifer->light_position[2] = { lucifer->current_pos.x + lucifer->Distance,  lucifer->current_pos.y - lucifer->Distance };
    lucifer->light_position[3] = { lucifer->current_pos.x + lucifer->Distance,  lucifer->current_pos.y + lucifer->Distance };
    lucifer->light_position[4] = { lucifer->current_pos.x - (lucifer->Distance  * lucifer->root2),  lucifer->current_pos.y };
    lucifer->light_position[5] = { lucifer->current_pos.x + (lucifer->Distance  * lucifer->root2),  lucifer->current_pos.y };
    lucifer->light_position[6] = { lucifer->current_pos.x, lucifer->current_pos.y - (lucifer->Distance * lucifer->root2)};
    lucifer->light_position[7] = { lucifer->current_pos.x, lucifer->current_pos.y + (lucifer->Distance * lucifer->root2) };
}

void Lucifer::Charging_light::Update(GameObject* object,[[maybe_unused]] float dt) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        lucifer->charge_delay += dt;
        if (lucifer->playerPosition.x + 10 > lucifer->GetPosition().x) {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        else if (lucifer->playerPosition.x - 10 <= lucifer->GetPosition().x) {
            lucifer->SetScale({ -lucifer->scale.x, lucifer->scale.y });
        }
        else {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        while (lucifer->is_charged == false && lucifer->charge_delay > 1.5) {
            for (int i = 0; i < 4; i++) {
                if (lucifer->create_count == 1 || lucifer->create_count == 3) {
                    lucifer->num = i + 4;
                }
                else {
                    lucifer->num = i;
                }
                float x = (lucifer->light_position[lucifer->num].x - lucifer->GetPosition().x);
                float y = (lucifer->light_position[lucifer->num].y - lucifer->GetPosition().y);
                vec2 direction_vector(x, y);
                direction_vector = normalize(direction_vector);
                direction_vector *= -550;
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Divine_light>>()->Shot(
                    1, vec2{ lucifer->light_position[lucifer->num].x, lucifer->light_position[lucifer->num].y }, direction_vector);
            }
            lucifer->create_count++;
            if (lucifer->create_count >= 4) {
                lucifer->is_charged = true;
            }
            lucifer->charge_delay = 0;
        }
    }
}

void Lucifer::Charging_light::CheckExit(GameObject* object)
{
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (lucifer->is_charged == true && lucifer->charge_delay > 3) {
        lucifer->change_state(&lucifer->judgment);
        SoundEffect::StopSoundEffect(SFX::Charging_ball);
        SoundEffect::Stop_Loop_SoundEffect(SFX::Charging_ball);
    }
}

void Lucifer::Judgment::Enter(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_created = false;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
    lucifer->current_pos = { lucifer->GetPosition() };
    lucifer->light_position[0] = { lucifer->current_pos.x - (lucifer->Distance),  lucifer->current_pos.y };
    lucifer->light_position[1] = { lucifer->current_pos.x, lucifer->current_pos.y - (lucifer->Distance) };
    lucifer->light_position[2] = { lucifer->current_pos.x + (lucifer->Distance),  lucifer->current_pos.y };
    lucifer->light_position[3] = { lucifer->current_pos.x, lucifer->current_pos.y + (lucifer->Distance) };
    SoundEffect::PlaySoundEffect(SFX::Angel_laser);
    SoundEffect::SetSoundEffectVolume(SFX::Angel_laser, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::Angel_laser);
}

void Lucifer::Judgment::Update(GameObject* object, float dt) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        lucifer->attack_delay += dt;

        if (lucifer->playerPosition.x + 10 > lucifer->GetPosition().x) {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        else if (lucifer->playerPosition.x - 10 <= lucifer->GetPosition().x) {
            lucifer->SetScale({ -lucifer->scale.x, lucifer->scale.y });
        }
        else {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }

        if (lucifer->is_created == false) {
            for (int i = 0; i < 4; i++) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::FinalSpark>>()->Show
                (1, vec2(lucifer->light_position[i].x, lucifer->light_position[i].y), lucifer->angle * Math::PI);
                lucifer->angle += 0.5;
            }
            lucifer->is_created = true;
        }
    }
}

void Lucifer::Judgment::CheckExit([[maybe_unused]] GameObject* object){
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (lucifer->attack_delay >= 10) {//
        SoundEffect::StopSoundEffect(SFX::Angel_laser);
        SoundEffect::Stop_Loop_SoundEffect(SFX::Angel_laser);
        lucifer->change_state(&lucifer->holy_sword);
    }
}

void Lucifer::Holy_Sword::Enter(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_created = false;
    lucifer->particle_can_move = false;
    lucifer->create_count = 0;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
}

void Lucifer::Holy_Sword::Update(GameObject* object, float dt) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        float orbitRadius = 250.0f;
        lucifer->attack_delay += dt;

        if (lucifer->playerPosition.x + 10 > lucifer->GetPosition().x) {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        else if (lucifer->playerPosition.x - 10 <= lucifer->GetPosition().x) {
            lucifer->SetScale({ -lucifer->scale.x, lucifer->scale.y });
        }
        else {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }

        while (lucifer->attack_delay >= 0.1 && lucifer->is_created == false) {
            float orbitX = lucifer->GetPosition().x + orbitRadius * cos(lucifer->angle * Math::PI);
            float orbitY = lucifer->GetPosition().y + orbitRadius * sin(lucifer->angle * Math::PI);

            lucifer->particle_vec[lucifer->create_count] = Math::RotationMatrix(lucifer->angle * Math::PI) * vec2 { 1200, 0 };

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Light_spear>>()->Rotate_Shot
            (1, vec2{ orbitX, orbitY }, vec2{ 0,0 }, lucifer->angle * Math::PI);
            lucifer->angle += 0.25;//
            lucifer->attack_delay = 0;
            lucifer->create_count++;

            if (lucifer->create_count >= 8) {
                lucifer->is_created = true;
                lucifer->particle_can_move = true;
                SoundEffect::PlaySoundEffect(SFX::Angel_spear);
                SoundEffect::SetSoundEffectVolume(SFX::Angel_spear, SOUND);
            }
        }
    }
}

void Lucifer::Holy_Sword::CheckExit([[maybe_unused]] GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (lucifer->attack_delay >= 2) {//
        lucifer->change_state(&lucifer->javelin);
    }
}

void Lucifer::Javelin::Enter(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_created = false;
    lucifer->create_count = 0;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
}

void Lucifer::Javelin::Update(GameObject* object, float dt) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        if (lucifer->playerPosition.x + 10 > lucifer->GetPosition().x) {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        else if (lucifer->playerPosition.x - 10 <= lucifer->GetPosition().x) {
            lucifer->SetScale({ -lucifer->scale.x, lucifer->scale.y });
        }
        else {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }

        lucifer->attack_delay += dt;
        vec2 luciferPosition = lucifer->GetPosition();
        vec2 relativePosition = lucifer->playerPosition - luciferPosition;
        lucifer->angle = atan2(relativePosition.y, relativePosition.x);
        float timer = 0.4f;
        float timerbox[7] = { timer / 2, timer, timer / 2, timer, timer / 2, timer / 2, timer / 2 };

        while (lucifer->attack_delay >= timerbox[lucifer->create_count] && lucifer->is_created == false) {

            lucifer->particle_vec2 = Math::RotationMatrix(lucifer->angle) * vec2 { 1200, 0 };

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Javelin>>()->Rotate_Shot
            (1, vec2{ lucifer->GetPosition() }, vec2{ lucifer->particle_vec2 }, lucifer->angle);
            SoundEffect::PlaySoundEffect(SFX::Angel_spear);
            SoundEffect::SetSoundEffectVolume(SFX::Angel_spear, SOUND);
            lucifer->attack_delay = 0;
            lucifer->create_count++;

            if (lucifer->create_count >= 6) {
                lucifer->is_created = true;
            }
        }
    }
}

void Lucifer::Javelin::CheckExit([[maybe_unused]] GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (lucifer->attack_delay >= 3) {//
        lucifer->change_state(&lucifer->teleport);
    }
}

void Lucifer::Teleport::Enter(GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::teleport));
    lucifer->is_created = false;
    lucifer->is_teleported = false;
    lucifer->is_animation_ended = false;
    lucifer->create_count = 0;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
    lucifer->pattern_count++;
    SoundEffect::PlaySoundEffect(SFX::Angel_teleport);
    SoundEffect::SetSoundEffectVolume(SFX::Angel_teleport, SOUND);
}

void Lucifer::Teleport::Update(GameObject* object, float dt) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        if (lucifer->playerPosition.x + 10 > lucifer->GetPosition().x) {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }
        else if (lucifer->playerPosition.x - 10 <= lucifer->GetPosition().x) {
            lucifer->SetScale({ -lucifer->scale.x, lucifer->scale.y });
        }
        else {
            lucifer->SetScale({ lucifer->scale.x, lucifer->scale.y });
        }

        if (lucifer->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::teleport) && lucifer->is_teleported == false)
        {
            if (lucifer->GetGOComponent<Sprite>()->AnimationEnded()) {
                lucifer->SetPosition(lucifer->teleport_place());
                lucifer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::teleport));
                lucifer->is_teleported = true;
            }
        }

        if (lucifer->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::teleport) && lucifer->is_teleported == true)
        {
            if (lucifer->GetGOComponent<Sprite>()->AnimationEnded()) {
                lucifer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
                lucifer->is_animation_ended = true;//
            }
        }

        if (lucifer->is_animation_ended == true) {
            lucifer->attack_delay += dt;
        }
        vec2 luciferPosition = lucifer->GetPosition();
        vec2 relativePosition = lucifer->playerPosition - luciferPosition;
        lucifer->angle = atan2(relativePosition.y, relativePosition.x);

        while (lucifer->attack_delay >= 0.2 && lucifer->is_created == false) {

            lucifer->particle_vec2 = Math::RotationMatrix(lucifer->angle) * vec2 { 1200, 0 };

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Javelin>>()->Rotate_Shot
            (1, vec2{ lucifer->GetPosition() }, vec2{ lucifer->particle_vec2 }, lucifer->angle);
            SoundEffect::PlaySoundEffect(SFX::Angel_spear);
            SoundEffect::SetSoundEffectVolume(SFX::Angel_spear, SOUND);
            lucifer->attack_delay = 0;
            lucifer->create_count++;

            if (lucifer->create_count >= 3) {
                lucifer->is_created = true;
            }
        }
    }
}

void Lucifer::Teleport::CheckExit([[maybe_unused]] GameObject* object) {
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    if (lucifer->attack_delay >= 3) {//
        if (lucifer->pattern_count < 2) {
            lucifer->change_state(&lucifer->teleport);
        }
        else {
            lucifer->pattern_count = 0;
            lucifer->change_state(&lucifer->state_idle);
        }
    }
}

//Player Down!!!!
void Lucifer::Player_Down::Enter([[maybe_unused]] GameObject* object)
{
    Lucifer* lucifer = static_cast<Lucifer*>(object);
    lucifer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    lucifer->stop_move = true;
    lucifer->SetVelocity(vec2(0, 0));
}

void Lucifer::Player_Down::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {}

void Lucifer::Player_Down::CheckExit([[maybe_unused]] GameObject* object) {}
