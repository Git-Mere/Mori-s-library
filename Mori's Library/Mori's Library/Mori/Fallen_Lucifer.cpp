#include "Fallen_Lucifer.h"
#include "../Engine/Particels.h"
#include "../Engine/Constant.h"
#include "Final_Spark.h"
#include "Angel_wings.h"
#include "Light_spear.h"
#include "Javelin.h"
#include "Life.h"
#include "Fallen_Attack.h"

extern float SOUND;
Fallen_Lucifer::Fallen_Lucifer(vec2 position, Player& player) : GameObject(position)
{
    AddGOComponent(new Sprite("assets/sprite/Bible/Fallen_Angel.spt", this));
    SetScale(scale);
    hurt_timer = new DownTimer(0.0f);
    player_ = &player;
    current_state = &state_idle;
    current_state->Enter(this);
    SetCollisionScale(vec2{ ColSize }, true);
    AddGOComponent(new Life(5));
    is_dead = false;
}

void Fallen_Lucifer::Update(float dt)
{
    playerPosition = player_->GetPosition();
    GameObject::Update(dt);
    hurt_timer->Update(dt);
    if (hurt_timer->Remaining() < 0.5) {
        stop_move = false;
    }
    if (hurt_timer->Remaining() == 0.0) {
        if(current_state != &state_running){ SetVelocity(vec2(0, 0)); }
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

bool Fallen_Lucifer::Fallen_LuciferDead() {
    return is_dead;
}

void Fallen_Lucifer::Draw(const mat3& world_to_ndc)
{
    if (is_hurt == false || hurt_timer->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }
}

bool Fallen_Lucifer::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    return false;
}

void Fallen_Lucifer::ResolveCollision(GameObject* other_object)
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
            GetGOComponent<Life>()->Sub(1);
            SoundEffect::PlaySoundEffect(SFX::Devil_Hurt);
            SoundEffect::SetSoundEffectVolume(SFX::Devil_Hurt, SOUND);
            hurt_timer->Set(hurt_time);
            is_hurt = true;
            player_->can_attack = false;
            player_->parrying_count = 0;
        }
        break;
    };
}

const vec2 Fallen_Lucifer::Return_coordinate()
{
    return GetPosition();
}

void Fallen_Lucifer::update_velocity(float dt) {
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

vec2 Fallen_Lucifer::teleport_place()
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

/////////////////////////////////// State Idle ///////////////////////////////////
void Fallen_Lucifer::State_Idle::Enter([[maybe_unused]] GameObject* object) {
}

void Fallen_Lucifer::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void Fallen_Lucifer::State_Idle::CheckExit(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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

/////////////////////////////////// State Running ///////////////////////////////////
void Fallen_Lucifer::State_Running::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->attack_delay = 0;
}

void Fallen_Lucifer::State_Running::Update(GameObject* object, float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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

void Fallen_Lucifer::State_Running::CheckExit(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->attack_delay > 3) {
        switch (pattern_number) {
        case 0:
            pattern_number++;
            lucifer->change_state(&lucifer->dark_sword);//dark_sword
            break;

        case 1:
            pattern_number++;
            lucifer->change_state(&lucifer->state_hell);//state_hell
            break;

        case 2:
            pattern_number++;
            lucifer->change_state(&lucifer->charging_dark);//charging_dark
            break;
        case 3:
            pattern_number = 0;
            lucifer->change_state(&lucifer->judgment);//judgment
            break;
        }
    }
}

/////////////////////////////////// State_Hell ///////////////////////////////////
void Fallen_Lucifer::State_Hell::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->attack_delay = 0;
    Pillar_count = 0;
    Pit_count = 0;
    pillar_timer = pillar_timer_max;
}

void Fallen_Lucifer::State_Hell::Update(GameObject* object, float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (!lucifer->GetGOComponent<Life>()->Dead()) {
        lucifer->attack_delay += dt;
        pillar_timer += dt;
        Pit_timer += dt;
    }

    if (Pillar_count < Pillar_count_max && pillar_timer > pillar_timer_max) {
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Fire_Pillar>>()->Rotate_Shot(1,  vec2(-Engine::Instance()->Get_Window().x,  Engine::Instance()->Get_Window().y / 2), vec2(300.0f, 0.0f), Math::PI);
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Fire_Pillar>>()->Emit(1,  vec2( Engine::Instance()->Get_Window().x, -Engine::Instance()->Get_Window().y / 2), vec2(-300.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
        SoundEffect::PlaySoundEffect(SFX::Devil_Floor_Sword); // 이거 플레이어가 필러랑 겹쳐서 지나갔을때만 나오게 되는지는 모르겠음 ㅇㅅㅇ.
        SoundEffect::SetSoundEffectVolume(SFX::Devil_Floor_Sword, SOUND);
        SoundEffect::Loop_SoundEffect(SFX::Devil_Floor_Sword);

        Pillar_count += 2;
        pillar_timer = 0;
    }

    if (Pit_count < Pit_count_max) {
        util::using_random_seed();
        float position_x1 = util::random(-Engine::Instance()->Get_Window().x, Engine::Instance()->Get_Window().x);
        float position_y1 = util::random(-Engine::Instance()->Get_Window().y, Engine::Instance()->Get_Window().y);
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Fire_Pit>>()->Emit(1, vec2(position_x1, position_y1), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
        if (!lucifer->sound_one) {
            SoundEffect::PlaySoundEffect(SFX::Devil_Floor_bomb);
            SoundEffect::SetSoundEffectVolume(SFX::Devil_Floor_bomb, SOUND);
            SoundEffect::Loop_SoundEffect(SFX::Devil_Floor_bomb);
            lucifer->sound_one = true;
        }
        Pit_count += 1;
        Pit_timer = 0;
    }
    if (Pillar_count < Pillar_count_max && Pit_count == Pit_count_max && Pit_timer > Pit_timer_max) {
        
        Pit_count = 0;
        Pit_timer = 0;
    }
    
}

void Fallen_Lucifer::State_Hell::CheckExit(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->attack_delay > 15.0 && Pillar_count == Pillar_count_max) {
    SoundEffect::StopSoundEffect(SFX::Devil_Floor_Sword);
    SoundEffect::Stop_Loop_SoundEffect(SFX::Devil_Floor_Sword);
    SoundEffect::StopSoundEffect(SFX::Devil_Floor_bomb);
    SoundEffect::Stop_Loop_SoundEffect(SFX::Devil_Floor_bomb);
    lucifer->sound_one = false;
        lucifer->change_state(&lucifer->javelin);
    }
}
///////////////////////////////////  ///////////////////////////////////





///////////////////////////////////  Fallen_Javelin ///////////////////////////////////
void Fallen_Lucifer::Fallen_Javelin::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_created = false;
    lucifer->create_count = 0;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;

    lucifer->light_position[0] = { lucifer->current_pos.x - (lucifer->Distance),  lucifer->current_pos.y };
    lucifer->light_position[2] = { lucifer->current_pos.x + (lucifer->Distance),  lucifer->current_pos.y };
}

void Fallen_Lucifer::Fallen_Javelin::Update(GameObject* object, float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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
        float timer = 0.5f;
        float timerbox[7] = { timer / 3, timer / 2, timer, timer, timer / 2, timer / 3, timer / 2 };

        while (lucifer->attack_delay >= timerbox[lucifer->create_count] && lucifer->is_created == false) {

            lucifer->particle_vec2 = Math::RotationMatrix(lucifer->angle) * vec2 { 1200, 0 };

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Dark_Javelin>>()->Rotate_Shot
            (1, vec2{ lucifer->GetPosition() }, vec2{ lucifer->particle_vec2 }, lucifer->angle);
            SoundEffect::PlaySoundEffect(SFX::Devil_spear);
            SoundEffect::SetSoundEffectVolume(SFX::Devil_spear, SOUND);
            lucifer->attack_delay = 0;
            lucifer->create_count++;

            if (lucifer->create_count >= 6) {
                lucifer->is_created = true;
            }
        }

        if (2.0 >= lucifer->attack_delay && lucifer->attack_delay >= 1.5) {
            float orbitRadius = 750.0f;
            pre_angle = lucifer->angle;
            orbitX = lucifer->GetPosition().x + orbitRadius * cos(lucifer->angle );
            orbitY = lucifer->GetPosition().y + orbitRadius * sin(lucifer->angle );
        }
    }
}

void Fallen_Lucifer::Fallen_Javelin::CheckExit([[maybe_unused]] GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->attack_delay >= 2.5) {
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Lazer>>()->Show(
            1,
            vec2(orbitX, orbitY),
            pre_angle
        );
        SoundEffect::PlaySoundEffect(SFX::Devil_One_laser);
        SoundEffect::SetSoundEffectVolume(SFX::Devil_One_laser, SOUND);
        lucifer->change_state(&lucifer->state_idle);
    }
}

//////////////////////////////////////////////////////////// Charge Dark ////////////////////////////////////////////////////////////
void Fallen_Lucifer::Charging_Dark::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    SoundEffect::PlaySoundEffect(SFX::Devil_charging_ball);
    SoundEffect::SetSoundEffectVolume(SFX::Devil_charging_ball, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::Devil_charging_ball);
    lucifer->charge_delay = 0;
    lucifer->create_count = 0;
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_charged = false;
    lucifer->current_pos = { lucifer->GetPosition() };
    lucifer->light_position[0] = { lucifer->current_pos.x - lucifer->Distance,  lucifer->current_pos.y - lucifer->Distance };
    lucifer->light_position[1] = { lucifer->current_pos.x - lucifer->Distance,  lucifer->current_pos.y + lucifer->Distance };
    lucifer->light_position[2] = { lucifer->current_pos.x + lucifer->Distance,  lucifer->current_pos.y - lucifer->Distance };
    lucifer->light_position[3] = { lucifer->current_pos.x + lucifer->Distance,  lucifer->current_pos.y + lucifer->Distance };
    lucifer->divine_dark_angle[0] = Math::PI * 2.25; //왼쪽아래
    lucifer->divine_dark_angle[1] = Math::PI * 1.75; //왼쪽위
    lucifer->divine_dark_angle[2] = Math::PI * 1.75; //오른쪽 아래
    lucifer->divine_dark_angle[3] = Math::PI * 2.25; //오른쪽 위

    lucifer->light_position[4] = { lucifer->current_pos.x - (lucifer->Distance * lucifer->root2),  lucifer->current_pos.y };
    lucifer->light_position[5] = { lucifer->current_pos.x + (lucifer->Distance * lucifer->root2),  lucifer->current_pos.y };
    lucifer->light_position[6] = { lucifer->current_pos.x, lucifer->current_pos.y - (lucifer->Distance * lucifer->root2) };
    lucifer->light_position[7] = { lucifer->current_pos.x, lucifer->current_pos.y + (lucifer->Distance * lucifer->root2) };

    lucifer->divine_dark_angle[4] = Math::PI * 0.0f; //왼
    lucifer->divine_dark_angle[5] = Math::PI * 0.0f; //오
    lucifer->divine_dark_angle[6] = Math::PI * 2.5f; //위
    lucifer->divine_dark_angle[7] = Math::PI * 2.5f; //아래
}

void Fallen_Lucifer::Charging_Dark::Update(GameObject* object, [[maybe_unused]] float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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
                if (lucifer->num != 4 && lucifer->num != 0 && lucifer->num != 1 && lucifer->num != 6) {
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Divine_Dark>>()->Rotate_Shot(
                        1, vec2{ lucifer->light_position[lucifer->num].x, lucifer->light_position[lucifer->num].y }, direction_vector, lucifer->divine_dark_angle[lucifer->num]);
                }
                else {
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Divine_Dark2>>()->Rotate_Shot(
                        1, vec2{ lucifer->light_position[lucifer->num].x, lucifer->light_position[lucifer->num].y }, direction_vector, lucifer->divine_dark_angle[lucifer->num]);
                }
            }
            lucifer->create_count++;
            if (lucifer->create_count >= 4) {
                lucifer->is_charged = true;
            }
            lucifer->charge_delay = 0;
        }
    }
}

void Fallen_Lucifer::Charging_Dark::CheckExit(GameObject* object)
{
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->is_charged == true && lucifer->charge_delay > 3) {
        SoundEffect::StopSoundEffect(SFX::Devil_charging_ball);
        SoundEffect::Stop_Loop_SoundEffect(SFX::Devil_charging_ball);
        lucifer->change_state(&lucifer->javelin);
    }
}

//////////////////////////////////////////////////////////// Holy Dark ////////////////////////////////////////////////////////////
void Fallen_Lucifer::Dark_Sword::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_created = false;
    lucifer->particle_can_move = false;
    lucifer->create_count = 0;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
    lucifer->dark_sword_count++;
}

void Fallen_Lucifer::Dark_Sword::Update(GameObject* object, float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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

        while (lucifer->attack_delay >= 0.05 && lucifer->is_created == false) {
            float orbitX = lucifer->GetPosition().x + orbitRadius * cos(lucifer->angle * Math::PI);
            float orbitY = lucifer->GetPosition().y + orbitRadius * sin(lucifer->angle * Math::PI);

            lucifer->particle_vec[lucifer->create_count] = Math::RotationMatrix(lucifer->angle * Math::PI) * vec2 { 1200, 0 };

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Dark_Spear>>()->Rotate_Shot
            (1, vec2{ orbitX, orbitY }, vec2{ 0,0 }, lucifer->angle * Math::PI);

            lucifer->angle += 0.125;
            lucifer->attack_delay = 0;
            lucifer->create_count++;

            if (lucifer->create_count >= 16 ) {
                lucifer->is_created = true;
                lucifer->particle_can_move = true;
                SoundEffect::PlaySoundEffect(SFX::Devil_spear);
                SoundEffect::SetSoundEffectVolume(SFX::Devil_spear, SOUND);
            }

        }
    }
}

void Fallen_Lucifer::Dark_Sword::CheckExit([[maybe_unused]] GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->dark_sword_count >= 2) {
        if (lucifer->attack_delay >= 3) {
            lucifer->dark_sword_count = 0;
            lucifer->change_state(&lucifer->javelin);
        }
    }
    else {
        if (lucifer->attack_delay >= 2) {
            lucifer->change_state(&lucifer->dark_sword);
        }
    }
}

///////////////////////////////////////////////////
void Fallen_Lucifer::Judgment::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->is_created = false;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
    lucifer->current_pos = { lucifer->GetPosition() };
    SoundEffect::PlaySoundEffect(SFX::Devil_laser);
    SoundEffect::SetSoundEffectVolume(SFX::Devil_laser, SOUND);
    SoundEffect::Loop_SoundEffect(SFX::Devil_laser);
}

void Fallen_Lucifer::Judgment::Update(GameObject* object, float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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
            float angleIncrement = Math::PI / 4.0;
            for (int i = 0; i < 8; i++) {
                float lazer_angle = lucifer->angle + angleIncrement * i;
                lucifer->light_position[i] = {
                    lucifer->current_pos.x + lucifer->Distance * cos(lazer_angle),
                    lucifer->current_pos.y + lucifer->Distance * sin(lazer_angle)
                };
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::DarkSpark>>()->Show
                (1, vec2(lucifer->light_position[i].x, lucifer->light_position[i].y), lazer_angle);
                
            }
            lucifer->is_created = true;
        }

    }
}

void Fallen_Lucifer::Judgment::CheckExit([[maybe_unused]] GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->attack_delay >= 10) {
        SoundEffect::StopSoundEffect(SFX::Devil_laser);
        SoundEffect::Stop_Loop_SoundEffect(SFX::Devil_laser);
        lucifer->change_state(&lucifer->teleport);
    }
}
////////////////////////////////////////// Teleport //////////////////////////////////////////

void Fallen_Lucifer::Teleport::Enter(GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->SetVelocity({ 0,0 });
    lucifer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::teleport));
    util::using_random_seed();
    lucifer->is_created = false;
    lucifer->is_teleported = false;
    lucifer->is_animation_ended = false;
    lucifer->create_count = 0;
    lucifer->angle = 0;
    lucifer->attack_delay = 0;
    SoundEffect::PlaySoundEffect(SFX::Devil_teleport);
    SoundEffect::SetSoundEffectVolume(SFX::Devil_teleport, SOUND);
}

void Fallen_Lucifer::Teleport::Update(GameObject* object, float dt) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
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
                lucifer->is_animation_ended = true;
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

            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Dark_Javelin>>()->Rotate_Shot
            (1, vec2{ lucifer->GetPosition() }, vec2{ lucifer->particle_vec2 }, lucifer->angle);
            SoundEffect::PlaySoundEffect(SFX::Devil_spear);
            SoundEffect::SetSoundEffectVolume(SFX::Devil_spear, SOUND);
            lucifer->attack_delay = 0;
            lucifer->create_count++;

            if (lucifer->create_count >= 3) {
                lucifer->is_created = true;
            }
        }
    }
}

void Fallen_Lucifer::Teleport::CheckExit([[maybe_unused]] GameObject* object) {
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    if (lucifer->attack_delay >= 3) {
        lucifer->change_state(&lucifer->state_idle);
    }
}

//Player Down!!!!
void Fallen_Lucifer::Player_Down::Enter([[maybe_unused]] GameObject* object)
{
    Fallen_Lucifer* lucifer = static_cast<Fallen_Lucifer*>(object);
    lucifer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    lucifer->stop_move = true;
    lucifer->SetVelocity(vec2(0, 0));
}

void Fallen_Lucifer::Player_Down::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {}

void Fallen_Lucifer::Player_Down::CheckExit([[maybe_unused]] GameObject* object) {}
