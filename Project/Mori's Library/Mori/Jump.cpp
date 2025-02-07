#include "../Engine/Skill.h"

Jump::Jump(Input::Keys key, Player& player, float cooltime, float height) : Skill(key, player, cooltime), height(height)
{
}

void Jump::func()
{
	SetVelocity(vec2(player->GetVelocity().x, height));
}


