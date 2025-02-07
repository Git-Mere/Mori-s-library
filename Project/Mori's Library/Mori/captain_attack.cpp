#include "captain_attack.h"
#include "../Engine/Constant.h"
#include "vec2.h"
#include "../Engine/Collision.h"
#include "../Engine/Particels.h"
#include "States.h"
#include "Life.h"
#include "../ScreenEffect.h"
extern float SOUND;
Captain_attack::Captain_attack(vec2 position, Player& player, Captain& captain) : GameObject(position), play_ptr(&player), captain_ptr(&captain)
{
    AddGOComponent(new Sprite("assets/sprite/Book3/Captain_attack.spt", this));
    if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
        Load("assets/textfile/captain_attack_easy.txt");
    }
    else {
        Load("assets/textfile/captain_attack.txt");
    }
    SetScale(right_left_size);
    SetRotation_Pivot(vec2(0, 150));
    current_state = &state_wait;
    current_state->Enter(this);
    SetCollisionScale(vec2{ 80, 420 }, true);
}

void Captain_attack::Update(float dt) {
    if (play_ptr->GetGOComponent<Life>()->Dead() == false) {
        if (finish == false) {
            GameObject::Update(dt);
        }
    }

#ifdef _DEBUG
    if (Engine::GetInput().KeyJustReleased(Input::Keys::R)) {
        index = 0;
        finish = false;
        current_state = &state_wait;
        current_state->Enter(this);
    }
#endif

}

bool Captain_attack::CanCollideWith(GameObjectTypes other_object_type)
{
    if (other_object_type == GameObjectTypes::Player) {
        return true;
    }
    return false;
}

void Captain_attack::ResolveCollision(GameObject* other_object)
{
    Player* player = static_cast<Player*>(other_object);
    if (player->Get_parrying_mode()) {
        success_parrying = true;

        Engine::GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(0.5f, 5, 5.f);

        if (current_state == &state_middle) {
            vec2 direction_vector = normalize(other_object->GetPosition() - GetPosition());
            SetVelocity(direction_vector * 10);
        }
    }
}

void Captain_attack::Load(const std::filesystem::path& pattern_file)
{
    std::ifstream in_file(pattern_file);

    std::string text;
    in_file >> text;

    float x = 0;
    float y = 0;
    std::pair<type, float> value;

    while (in_file.eof() == false) {
        if (text == "Wait") {
            value.first = Wait;
            in_file >> value.second;
            in_file >> x;
            in_file >> y;
            data.push_back(value);
            font_size_vec.push_back(vec2(x, y));
        }
        else if (text == "Left") {
            value.first = Left;
            in_file >> value.second;
            in_file >> x;
            in_file >> y;
            data.push_back(value);
            font_size_vec.push_back(vec2(x, y));
        }
        else if (text == "Right") {
            value.first = Right;
            in_file >> value.second;
            in_file >> x;
            in_file >> y;
            data.push_back(value);
            font_size_vec.push_back(vec2(x, y));
        }
        else if (text == "Mid") {
            value.first = Middle;
            in_file >> value.second;
            in_file >> x;
            in_file >> y;
            data.push_back(value);
            font_size_vec.push_back(vec2(x, y));
        }
        in_file >> text;
    }
    value.first = End;
    value.second = 0;
    x = 0;
    y = 0;
    font_size_vec.push_back(vec2(x, y));
    data.push_back(value);
}

vec2 Captain_attack::Get_font_size()
{
    return font_size_vec[index - 1];
}

void Captain_attack::re_position()
{
    SetScale(right_left_size);
    SetPosition(vec2(-40, 10));
    SetRotation(-1.f);
}

void Captain_attack::State_Wait::Enter(GameObject* obj)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    ca->success_parrying = false;
    timer.Set(ca->data[ca->index].second);
    ca->index++;
    ca->re_position();
    ca->captain_ptr->initialize();

    ca->captain_ptr->GetGOComponent<Sprite>()->PlayAnimation(0);
}

void Captain_attack::State_Wait::Update([[maybe_unused]] GameObject* obj, float dt)
{
    timer.Update(dt);
}

void Captain_attack::State_Wait::CheckExit(GameObject* obj)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    if (timer.end()) {
        if (ca->data[ca->index].first == type::Middle) {
            ca->change_state(&ca->state_middle);
        }
        else if (ca->data[ca->index].first == type::End) {
            ca->finish = true;
        }
        else if (ca->data[ca->index].first == type::Wait) {
            Enter(ca);
        }
        else {
            ca->change_state(&ca->state_leftright);
        }
    }
}


void Captain_attack::State_LeftRight::Enter(GameObject* obj)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    speed = ca->data[ca->index].second;
    _type = ca->data[ca->index].first;
    ca->index++;
    timer.Set(parrying_delay * (1 / speed));
    if (_type == type::Left) {
        ca->SetRotation(-2.f);
        ca->SetScale(ca->right_left_size);
    }
    else {
        ca->SetRotation(2.f);
        ca->SetScale(ca->right_left_size_minus);
    }
    SoundEffect::PlaySoundEffect(SFX::Throwing);
    SoundEffect::SetSoundEffectVolume(SFX::Throwing, SOUND);
}

void Captain_attack::State_LeftRight::Update(GameObject* obj, float dt)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    float velo = speed * dt;
    if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
        velo *= 0.7f;
    }
    if (ca->success_parrying) {
        timer.Update(dt);
        if (_type == type::Left) {
            ca->UpdateRotation(-velo * 3.f);
        }
        else {
            ca->UpdateRotation(velo * 3.f);
        }
    }
    else {
        if (_type == type::Left) {
            ca->UpdateRotation(velo * 1.5f);
        }
        else {
            ca->UpdateRotation(-velo * 1.5f);
        }
    }
}


void Captain_attack::State_LeftRight::CheckExit(GameObject* obj)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    if (_type == type::Left) {
        if (ca->GetRotation() >= 1.6f) {
            ca->change_state(&ca->state_wait);
        }
    }
    else {
        if (ca->GetRotation() <= -1.6f) {
            ca->change_state(&ca->state_wait);
        }
    }

    if (timer.Remaining() <= 0) {
        ca->change_state(&ca->state_wait);
    }

}

void Captain_attack::State_Middle::Enter(GameObject* obj)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    speed = ca->data[ca->index].second;
    if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
        speed *= 0.75;
    }
    ca->index++;

    ca->SetPosition(vec2(-40, 400));
    ca->SetVelocity(vec2(0, -speed));
    ca->SetRotation(0);
    
    ca->captain_ptr->middle_attack(-speed);


    timer.Set(parrying_delay);
    SoundEffect::PlaySoundEffect(SFX::Throwing);
    SoundEffect::SetSoundEffectVolume(SFX::Throwing, SOUND);

    ca->captain_ptr->GetGOComponent<Sprite>()->PlayAnimation(1);
}

void Captain_attack::State_Middle::Update(GameObject* obj, float dt)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);

    if (ca->success_parrying) {
        timer.Update(dt);

        ca->SetVelocity(vec2(0, speed * 2));
        ca->captain_ptr->parring(vec2(0, speed * 2));
        //ca->UpdateRotation(speed * dt);
    }
}

void Captain_attack::State_Middle::CheckExit(GameObject* obj)
{
    Captain_attack* ca = static_cast<Captain_attack*>(obj);
    if (timer.Remaining() <= 0) {
        ca->SetVelocity(vec2(0, 0));
        ca->change_state(&ca->state_wait);
    }
    if (ca->GetPosition().y < 0) {
        ca->SetVelocity(vec2(0, 0));
        ca->change_state(&ca->state_wait);
    }

}
