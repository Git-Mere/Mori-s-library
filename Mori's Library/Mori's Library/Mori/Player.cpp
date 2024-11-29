//#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/CameraCom.h"
#include "../Engine/Particels.h"
#include "../Engine/Constant.h"
#include "Player.h"
#include "Slime.h"
#include "Fiter.h"

#include <iostream>
#include <cmath>
#include "Final_Spark.h"
#include "Life.h"
#include "invincible.h"
#include "MagicBall.h"
#include "Fallen_Attack.h"
#include "FiterAttack.h"

#include "../Tuto_ball.h"
#include "../Engine/Gravity.h"
#include "../Engine/SkillManager.h"
#include "../Engine/Skill.h"
#include "../ScreenEffect.h"
#include "../Engine/Collision.h"

extern float SOUND;

Player::Player(vec2 position, vec2 scale, float rotation) : GameObject(position, rotation, scale) {
    AddGOComponent(new Sprite("assets/sprite/Player/Player.spt", this));

    shocktimer = new DownTimer(0);
    invin = new Invincible;
    skillmanager = new SkillManager;
   
    AddGOComponent(invin);
    AddGOComponent(shocktimer);
    AddGOComponent(skillmanager);
    AddGOComponent(new Life(5, true));

    skillmanager->AddComponent(new dash(Input::Keys::Z, *this, 0.3f, 1, dash_speed, 0.3f));
    skillmanager->AddComponent(new Parrying(Input::Keys::X, *this, 0.5f, 0.3f));

    SetScale(player_scale);
    SetCollisionScale(player_scale / 4, true);
    check_state();
    current_state = &state_idle;
    current_state->Enter(this);
}


bool Player::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    return true;
}

void Player::Draw(const mat3& world_to_ndc)
{
    if (invin->is_hurt() == false || invin->TickTock() == true) {
        GameObject::Draw(world_to_ndc);
    }
    skillmanager->GetComponent<Parrying>()->Draw(world_to_ndc);
}

void Player::Update(float dt)
{
    GameObject::Update(dt);
    cant_beyond_screen();

    if (what_state != States::Book3) {
        if (shocktimer->end()) {
            stop_move = false;
        }
        else {
            stop_move = true;
        }
    }

    if (parrying_count >= 5) {
        can_attack = true;
    }

    if (GetGOComponent<SkillManager>()->check_special_ani() == true) {
        special_ani = false;
    }
}

void Player::update_velocity(float dt) {
    if (stop_move == false) {
        if (Engine::GetInput().KeyDown(Input::Keys::Right)) {
            UpdateVelocity({ x_acceleration * dt, 0 });
            if (GetVelocity().x < 0) {
                SetVelocity({ 0, GetVelocity().y });
            }
            if (GetVelocity().x > max_velocity) {
                SetVelocity({ max_velocity, GetVelocity().y });
            }
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Left)) {
            UpdateVelocity({ -(x_acceleration * dt), 0 });
            if (GetVelocity().x > 0) {
                SetVelocity({ 0, GetVelocity().y });
            }
            if (GetVelocity().x < -max_velocity) {
                SetVelocity({ -max_velocity, GetVelocity().y });
            }
        }
        else {
            if (GetVelocity().x > x_drag * dt) {
                UpdateVelocity({ -(x_drag * dt), 0 });
            }
            else if (GetVelocity().x < -x_drag * dt) {
                UpdateVelocity({ x_drag * dt, 0 });
            }
            else {
                SetVelocity({ 0, GetVelocity().y });
            }
        }

        if (Engine::GetInput().KeyDown(Input::Keys::Up)) {
            UpdateVelocity({ 0, y_acceleration * dt });
            if (GetVelocity().y < 0) {
                SetVelocity({ GetVelocity().x, 0 });
            }
            if (GetVelocity().y > max_velocity) {
                SetVelocity({ GetVelocity().x, max_velocity });
            }
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Down)) {
            UpdateVelocity({ 0, -(y_acceleration * dt) });
            if (GetVelocity().y > 0) {
                SetVelocity({ GetVelocity().x, 0 });
            }
            if (GetVelocity().y < -max_velocity) {
                SetVelocity({ GetVelocity().x, -max_velocity });
            }
        }
        else {
            if (GetVelocity().y > y_drag * dt) {
                UpdateVelocity({ 0, -(y_drag * dt) });
            }
            else if (GetVelocity().y < -y_drag * dt) {
                UpdateVelocity({ 0 , y_drag * dt });
            }
            else {
                SetVelocity({ GetVelocity().x, 0 });
            }
        }
    }

    if (yes_gravity) {
        UpdateVelocity({ 0,-Engine::GetGameStateManager().GetGSComponent<Gravity>()->GetValue() * dt });
    }
}

void Player::update_move_direction()
{
    if (Engine::GetInput().KeyDown(Input::Keys::Up)) {
        move_direction.y = 1.0f;
    }
    else if (Engine::GetInput().KeyDown(Input::Keys::Down)) {
        move_direction.y = -1.0f;
    }
    else {
        move_direction.y = 0;
    }

    if (Engine::GetInput().KeyDown(Input::Keys::Right)) {
        move_direction.x = 1.0f;
    }
    else if (Engine::GetInput().KeyDown(Input::Keys::Left)) {
        move_direction.x = -1.0f;
    }
    else {
        move_direction.x = 0;
    }
}

void Player::determine_animation()
{
    if (special_ani == false) {
        if (Engine::GetInput().KeyDown(Input::Keys::Right)) {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Right));
            last_animation = static_cast<int>(Animations::Move_Right);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Left)) {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Left));
            last_animation = static_cast<int>(Animations::Move_Left);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Up)) {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Up));
            last_animation = static_cast<int>(Animations::Move_Up);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Down)) {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Down));
            last_animation = static_cast<int>(Animations::Move_Down);
        }
        else {
            if (last_animation == static_cast<int>(Animations::Move_Up)) {
                GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Look_Backward));
            }
            else if (last_animation == static_cast<int>(Animations::Move_Down)) {
                GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Look_Foward));
            }
            else if (last_animation == static_cast<int>(Animations::Move_Right)) {
                GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Look_Right));
            }
            else if (last_animation == static_cast<int>(Animations::Move_Left)) {
                GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Look_Left));
            }
            else {
                GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Look_Foward));
            }//

        }
    }
    else {
        if (Engine::GetInput().KeyDown(Input::Keys::Right)) {
            last_animation = static_cast<int>(Animations::Move_Right);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Left)) {
            last_animation = static_cast<int>(Animations::Move_Left);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Up)) {
            last_animation = static_cast<int>(Animations::Move_Up);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Down)) {
            last_animation = static_cast<int>(Animations::Move_Down);
        }
    }
}

void Player::ResolveCollision(GameObject* other_object)
{
    switch (other_object->Type()) {
    case GameObjectTypes::Slime:
    {
        shove(other_object);
        if (skillmanager->GetComponent<dash>()->now_delay() == false) {
            if (invin->is_invin() == false) {
                if (dynamic_cast<Slime*>(other_object)->Return_is_collide()) {
                    hurt(other_object, 1);
                }
            }
        }
        else if (skillmanager->GetComponent<dash>()->now_delay() == true) {
            other_object->ResolveCollision(this);
        }
    }
    break;

    case GameObjectTypes::slime_ball:
    {
        if (invin->is_invin() == false) {
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                shove(other_object);
                hurt(other_object, 1, true);
            }
        }
    }
    break;

    case GameObjectTypes::Block:
    {
        shove(other_object);
        SoundEffect::PlaySoundEffect(SFX::Blocking);
        SoundEffect::SetSoundEffectVolume(SFX::Blocking, SOUND);
    }
    break;

    case GameObjectTypes::MagicBall:
        if (static_cast<Particles::MagicBall*>(other_object)->GetCanCollision() && invin->is_invin() == false) {
            hurt(other_object, 1, false);
        }
        break;

    case GameObjectTypes::QuizBox:
        other_object->ResolveCollision(this);
        break;

    case GameObjectTypes::Fiter:
        if (dynamic_cast<Fiter*>(other_object)->Return_is_collide()) {
            shove(other_object);
            if (skillmanager->GetComponent<dash>()->now_delay() == false && invin->is_invin() == false) {
                hurt(other_object, 1);
            }
            else if (skillmanager->GetComponent<dash>()->now_delay() == true) {
                other_object->ResolveCollision(this);
                if (static_cast<Fiter*>(other_object)->GetBarrier() == false) {
                    static_cast<Fiter*>(other_object)->GetDamage();
                }
            }
        }
        break;

    case GameObjectTypes::ground:
    {
    }
    break;

    case GameObjectTypes::Captain_attack:
        if (invin->is_invin() == false) {
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                hurt(other_object, 1, false, 0.5f);
            }
        }
        break;

    case GameObjectTypes::Tuto_ball:
    {
        if (invin->is_invin() == false) {
            if (dynamic_cast<Particles::Tuto_ball*>(other_object)->Canparrying()) {
                skillmanager->GetComponent<Parrying>()->determine_success(other_object);
                if (Get_parrying_mode() == false) {
                    shove(other_object);
                }
            }
            else {
                shove(other_object);
            }

        }
    }
        break;

    case GameObjectTypes::Energy_Kick:
        if (invin->is_invin() == false) {
            hurt(other_object, 1);
        }
        break;

    case GameObjectTypes::Energy_Fist:
        if (invin->is_invin() == false) {
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                shove(other_object);
                hurt(other_object, 1, true);
            }
        }
        break;

    case GameObjectTypes::Energy_Lazer:
        if (static_cast<Particles::Energy_Lazer_L*>(other_object)->GetCanCollision() && invin->is_invin() == false ||
            static_cast<Particles::Energy_Lazer_R*>(other_object)->GetCanCollision() && invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1);
        }
        break;

    case GameObjectTypes::PunchEarthquake:
        if (invin->is_invin() == false) {
                shove(other_object);
                hurt(other_object, 1, true);
        }
        break;

    case GameObjectTypes::Lucifer:
    {
        shove(other_object);
        if (skillmanager->GetComponent<dash>()->now_delay() == false && invin->is_invin() == false) {
            hurt(other_object, 1);
        }
        if (skillmanager->GetComponent<dash>()->now_delay() == true && can_attack == true) {
            other_object->ResolveCollision(this);
        }
    }
    break;

    case GameObjectTypes::Fallen_Lucifer:
    {
        shove(other_object);
        if (skillmanager->GetComponent<dash>()->now_delay() == false && invin->is_invin() == false) {
            hurt(other_object, 1);
        }
        if (skillmanager->GetComponent<dash>()->now_delay() == true && can_attack == true) {
            other_object->ResolveCollision(this);
        }
    }
    break;

    case GameObjectTypes::divine_light:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
    }
    break;

    case GameObjectTypes::Divine_Dark:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
    }
    break;

    case GameObjectTypes::FinalSpark:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
    }
    break;

    case GameObjectTypes::Light_spear:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                hurt(other_object, 1);
            }
            else {
                other_object->ResolveCollision(this);
                parrying_count++;
                SoundEffect::PlaySoundEffect(SFX::Parring);
                SoundEffect::SetSoundEffectVolume(SFX::Parring, SOUND);
            }
        }
    }
    break;

    case GameObjectTypes::javelin:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                hurt(other_object, 1);
            }
            else {
                other_object->ResolveCollision(this);
                parrying_count++;
               SoundEffect::PlaySoundEffect(SFX::Parring);
               SoundEffect::SetSoundEffectVolume(SFX::Parring, SOUND);
            }
        }
    }
    break;

    case GameObjectTypes::Dark_Javelin:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                hurt(other_object, 1);
            }
            else {
                other_object->ResolveCollision(this);
                parrying_count++;
                SoundEffect::PlaySoundEffect(SFX::Parring);
                SoundEffect::SetSoundEffectVolume(SFX::Parring, SOUND);
            }
        }
    }
    break;

    case GameObjectTypes::Dark_Spear:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                hurt(other_object, 1);
            }
            else {
                other_object->ResolveCollision(this);
                parrying_count++;
                SoundEffect::PlaySoundEffect(SFX::Parring);
                SoundEffect::SetSoundEffectVolume(SFX::Parring, SOUND);
            }
        }
    }
    break;

    case GameObjectTypes::DarkSpark:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
    }
    break;

    case GameObjectTypes::Fire_Pillar:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
    }
    break;

    case GameObjectTypes::Fire_Pit:
        if (static_cast<Particles::Fire_Pit*>(other_object)->GetCanCollision() && invin->is_invin() == false) {
            hurt(other_object, 1);
        }
        break;

    case GameObjectTypes::Lazer:
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
        break;

    case GameObjectTypes::Portalbook:
    {
        Set_shock_timer(3.f);
        stop_skill = true;
        SetVelocity(vec2(0, 0));
    }
    break;

    case GameObjectTypes::Sphinx:
    {
        shove(other_object);
        if (skillmanager->GetComponent<dash>()->now_delay() == false && invin->is_invin() == false) {
            hurt(other_object, 1, false);
        }
        if (skillmanager->GetComponent<dash>()->now_delay() == true) {
            other_object->ResolveCollision(this);
        }
    }
    break;

    case GameObjectTypes::Earthquake:
    {
        if (static_cast<Particles::Earthquake*>(other_object)->GetCanCollision() && invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1, true);
        }
    }
    break;

    case GameObjectTypes::Tuto_dummy:
        shove(other_object);
        break;

    case GameObjectTypes::Wall:
        cant_beyond_wall(other_object);
        break;
    case GameObjectTypes::Mori:
        cant_beyond_wall(other_object);
        break;

    case GameObjectTypes::Bard:
    {
        shove(other_object);
        if (skillmanager->GetComponent<dash>()->now_delay() == false) {
            if (invin->is_invin() == false) {
                hurt(other_object, 1);
            }
        }
        else if (skillmanager->GetComponent<dash>()->now_delay() == true) {
            other_object->ResolveCollision(this);
        }

    }
    break;

    case GameObjectTypes::cosmic_tone:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            skillmanager->GetComponent<Parrying>()->determine_success(other_object);
            if (Get_parrying_mode() == false) {
                hurt(other_object, 1);
                reset_damage();
            }
            else {
                
                other_object->SetPosition(vec2(5000, 5000));

                other_object->ResolveCollision(this);
                parrying_count++;
                reset_combo_time();
                //sound_effects.at(SFX::Parring)->setvolume(50.0f);
                //sound_effects.at(SFX::Parring)->sound_play();
            }
        }
    }
    break;
    case GameObjectTypes::Boom:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1);
        }
    }
    break;
    case GameObjectTypes::electronic:
    {
        if (invin->is_invin() == false) {
            shove(other_object);
            hurt(other_object, 1);
        }
    }
    break;
    }
}

void Player::hurt(GameObject* other_object, int life_down, bool other_collision_on, float time)
{
    if (other_collision_on == true) {
        other_object->ResolveCollision(this);
    }
    invin->Set_invin(time, true);
    GetGOComponent<Life>()->Sub(life_down);
    SoundEffect::PlaySoundEffect(SFX::Player_heart);
    SoundEffect::SetSoundEffectVolume(SFX::Player_heart, SOUND);
    Engine::Instance()->GetGameStateManager().GetGSComponent<UIManager>()->SE_setting(Engine::hit, 3);
}

void Player::shove(GameObject* other_object)
{
    shocktimer->Set(0.3f);
    float x = (other_object->GetPosition().x - GetPosition().x) * 2;
    float y = (other_object->GetPosition().y - GetPosition().y) * 2;
    move_direction = { x , y };
    move_direction = normalize(move_direction);
    move_direction *= hurt_speed;
    SetVelocity(move_direction);
    move_direction *= 0;
}



vec2& Player::Get_move_dir()
{
    return move_direction;
}

void Player::Set_invin_timer(float time)
{
    invin->Set_invin(time, false);
}

void Player::Set_shock_timer(float time)
{
    shocktimer->Set(time);
}

void Player::Set_special_ani(bool value)
{
    if (special_ani == false) {
        special_ani = value;
    }
}

bool Player::Get_parrying_mode()
{
    return skillmanager->GetComponent<Parrying>()->Get_now_parring();
}

Skill* Player::Get_Skill(int i)
{
    return GetGOComponent<SkillManager>()->Get_Skill(i);
}

bool Player::Get_invin()
{
    return invin->end();
}

bool Player::Get_stop_skill()
{
    return stop_skill;
}

States Player::Get_what_state()
{
    return what_state;
}

int Player::Get_Life() { 
    return GetGOComponent<Life>()->GetLifeNumber();
}

void Player::change_player_scale(vec2 scale)
{
    player_scale = scale;
    SetScale(scale);
}


void Player::check_state()
{
    std::string tmp = Engine::GetGameStateManager().Get_current_state_name();
    if (tmp == "BookN") {
        what_state = States::BookN;
        skillmanager->RemoveComponent<dash>();
        yes_gravity = true;
        skillmanager->AddComponent(new Jump(Input::Keys::Z, *this, 3, 600));
    }
    else if (tmp == "Book1") {
        what_state = States::Book1;
    }
    else if (tmp == "Book2") {
        what_state = States::Book2;
    }
    else if (tmp == "Book3") {
        what_state = States::Book3;
        skillmanager->RemoveComponent<dash>();
    }
    else if (tmp == "libarary") {
        what_state = States::Libarary;
    }
    else if (tmp == "Bible") {
        what_state = States::Bible;
    }
    else if (tmp == "Fallen_Bible") {
        what_state = States::Fallen_Bible;
    }
    else if (tmp == "Tutorial") {
        what_state = States::Tutorial;
    }
    else if (tmp == "Book5") {
        what_state = States::Book5;
    }

}

void Player::cant_beyond_screen()
{
    vec2 col = GetCollisionScale();
    const vec2 player_position = GetPosition();
    const vec2 camera_position = Engine::GetGameStateManager().GetGSComponent<CameraComponent>()->GetPosition();
    const vec2 half_window_size = Engine::Instance()->Get_Window() / 2;

    const float left = ((camera_position.x - half_window_size.x) + (col.x / 2));
    const float right = ((camera_position.x + half_window_size.x) - (col.x / 2));
    const float bottom = ((camera_position.y - half_window_size.y) + (col.y / 2));
    const float top = ((camera_position.y + half_window_size.y) - (col.y / 2));

    if (player_position.x < left)
    {
        UpdatePosition({ left - player_position.x , 0.0 });
        SetVelocity({ 0.0, GetVelocity().y });
    }
    if (player_position.x > right)
    {
        UpdatePosition({ right - player_position.x, 0.0 });
        SetVelocity({ 0.0, GetVelocity().y });
    }
    if (player_position.y < bottom)
    {
        UpdatePosition({ 0.0 , bottom - player_position.y });
        SetVelocity({ GetVelocity().x, 0.0 });
    }
    if (player_position.y > top)
    {
        UpdatePosition({ 0.0, top - player_position.y });
        SetVelocity({ GetVelocity().x, 0.0 });
    }
}

void Player::cant_beyond_wall(GameObject* other_object)
{
    vec2 other_position = other_object->GetPosition();
    vec2 other_col = other_object->GetCollisionScale() / 2;
    vec2 player_position = GetPosition();
    vec2 player_col = GetCollisionScale() / 2;

    double left_x =     other_position.x - other_col.x;
    double right_x =    other_position.x + other_col.x;
    double up_y =       other_position.y + other_col.y;
    double down_y =     other_position.y - other_col.y;

    double pos_left_x = player_position.x - player_col.x;
    double pos_right_x = player_position.x + player_col.x;
    double pos_up_y = player_position.y + player_col.y;
    double pos_down_y = player_position.y - player_col.y;

    double left_diff = abs(pos_left_x - right_x);
    double right_diff = abs(left_x - pos_right_x);
    double up_diff = abs(pos_up_y - down_y);
    double down_diff = abs(up_y - pos_down_y);

    double min_diff = std::min(std::min(left_diff, right_diff), std::min(up_diff, down_diff));
    vec2 react_dir_x = normalize(vec2(other_position.x - player_position.x, 0));
    vec2 react_dir_y = normalize(vec2(0, other_position.y - player_position.y));
    if (min_diff == left_diff) {
        UpdatePosition(-react_dir_x);
        SetVelocity(-react_dir_x);
    }
    else if (min_diff == right_diff) {
        UpdatePosition(-react_dir_x);
        SetVelocity(-react_dir_x);
    }
    else if (min_diff == up_diff) {
        UpdatePosition(-react_dir_y);
        SetVelocity(-react_dir_y);
    }
    else if (min_diff == down_diff) {
        UpdatePosition(-react_dir_y);
        SetVelocity(-react_dir_y);
    }
}

void Player::State_Idle::Enter([[maybe_unused]] GameObject* object) {
    Player* player = static_cast<Player*>(object);
    if (player->sound_on) {
        //StopSoundEffect(SFX::Player_walk);
    }
}

void Player::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    Player* player = static_cast<Player*>(object);
    player->update_velocity(dt);
    player->update_move_direction();
    player->determine_animation();
}

void Player::State_Idle::CheckExit(GameObject* object) {
    Player* player = static_cast<Player*>(object);
    if (player->stop_move == false) {
        if (Engine::GetInput().KeyDown(Input::Keys::Up)) {
            player->change_state(&player->state_running);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Left)) {
            player->change_state(&player->state_running);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Down)) {
            player->change_state(&player->state_running);
        }
        else if (Engine::GetInput().KeyDown(Input::Keys::Right)) {
            player->change_state(&player->state_running);
        }
    }
}

void Player::State_Running::Enter(GameObject* object)
{
    Player* player = static_cast<Player*>(object);
    vec2 tmp = player->player_scale;
    if (!player->sound_on) {
        SoundEffect::PlaySoundEffect(SFX::Player_walk);
        SoundEffect::Loop_SoundEffect(SFX::Player_walk);
        SoundEffect::SetSoundEffectVolume(SFX::Player_walk, SOUND);
        player->sound_on = true;

    }
}

void Player::State_Running::Update(GameObject* object, float dt) {
    Player* player = static_cast<Player*>(object);
    vec2 tmp = player->player_scale;
    player->update_velocity(dt);
    player->update_move_direction();
    player->determine_animation();
}

void Player::State_Running::CheckExit(GameObject* object)
{
    Player* player = static_cast<Player*>(object);
    if (player->GetVelocity().x == 0 && player->GetVelocity().y == 0 )
    {
        if (player->sound_on) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::Player_walk);
            player->sound_on = false;
        }
        player->change_state(&player->state_idle);
    }
}


