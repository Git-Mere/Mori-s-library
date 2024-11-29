#include "Skill.h"

Skill::Skill(Input::Keys key, Player& player, float cooltime, float delay) : player(&player), key(key), cooltimer(cooltime), delay(delay) {
    cooltimer.ZeroSet();
}

bool Skill::use()
{
    if (cooltimer.end() && !player->Get_stop_skill()) {
        if (Engine::GetInput().KeyJustPressed(key)) {
            cooltimer.Reset();
            delay.Reset();
            return true;
        }
    }
	return false;
}

bool Skill::now_cooltime()
{
    return !cooltimer.end();
}

bool Skill::now_delay()
{
    return !delay.end();
}


void Skill::Cooling(float dt)
{
    cooltimer.Update(dt);
    delay.Update(dt);
}

void Skill::change_config(float cool, float del)
{
    cooltimer.Set(cool);
    delay.Set(del);
}








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Skill::SetPosition(vec2 new_position) {
    player->SetPosition(new_position);
}

void Skill::SetVelocity(vec2 new_position)
{
    player->SetVelocity(new_position);
}

void Skill::SetScale(vec2 new_scale)
{
    player->SetScale(new_scale);
}

void Skill::SetRotation(float new_rotation)
{
    player->SetRotation(new_rotation);
}

void Skill::SetCollisionScale(vec2 Collision_scale, bool is_chage) {
    player->SetCollisionScale(Collision_scale, is_chage);
}

void Skill::SetShaderName(Engine::ShaderName input)
{
    player->SetShaderName(input);
}