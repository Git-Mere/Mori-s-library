#include "../Engine/Skill.h"
#include "../Engine/Particels.h"
#include "../Engine/Constant.h"

extern float SOUND;

dash::dash(Input::Keys key, Player& player, float delay, float cooltime, float dash_speed, float invin_time) : Skill(key, player, cooltime, delay), dash_speed(dash_speed), invin_time(invin_time)
{
}

void dash::func()
{
	if (player->Get_move_dir().x == 0 && player->Get_move_dir().y == 0) {
		SetVelocity(vec2(1,1) * 0);
	}
	else {
		SetVelocity(normalize(player->Get_move_dir()) * dash_speed);
		Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Smoke>>()->Emit(1, vec2(player->GetPosition()), vec2(-player->Get_move_dir() * 50), vec2(-player->Get_move_dir()), static_cast<float>(Math::PI));
		SoundEffect::PlaySoundEffect(SFX::Dash);
		SoundEffect::SetSoundEffectVolume(SFX::Dash, SOUND);
	}

	player->Set_special_ani(true);
	vec2 player_dir = player->Get_move_dir();
	if (player_dir == vec2(1, 1) || player_dir == vec2(1, 0) || player_dir == vec2(1, -1)) {
		player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Right));
	}
	else if (player_dir == vec2(0, -1)) {
		player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Down));
	}
	else if (player_dir == vec2(-1, -1) || player_dir == vec2(-1, 0) || player_dir == vec2(-1, 1)) {
		player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Left));
	}
	else if (player_dir == vec2(0, 1)) {
		player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Up));
	}
	player->Set_invin_timer(invin_time);
	player->Set_shock_timer(invin_time);
}