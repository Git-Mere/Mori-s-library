#include "../Engine/Skill.h"
#include "../Mori/Player.h"
#include "Soundeffect.h"
extern float SOUND;
Parrying::Parrying(Input::Keys key, Player& player, float cooltime, float delay) : Skill(key, player, cooltime, delay), 
effect(new Sprite("assets/sprite/Parrying_effect.spt"))
{
	effect->Setframe(0, 6);
}

void Parrying::func()
{
	player->Set_special_ani(true);
	if (player->Get_invin()) {
		if (direction == 1 || direction == 2 || direction == 3) {
			SoundEffect::PlaySoundEffect(SFX::Just_Parring);
			SoundEffect::SetSoundEffectVolume(SFX::Just_Parring, SOUND);
			player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Parrying_Right));
		}
		else if (direction == 4) {
			SoundEffect::PlaySoundEffect(SFX::Just_Parring);
			SoundEffect::SetSoundEffectVolume(SFX::Just_Parring, SOUND);
			player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Parrying_Down));
		}
		else if (direction == 5 || direction == 6 || direction == 7) {
			SoundEffect::PlaySoundEffect(SFX::Just_Parring);
			SoundEffect::SetSoundEffectVolume(SFX::Just_Parring, SOUND);
			player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Parrying_Left));
		}
		else if (direction == 8) {
			SoundEffect::PlaySoundEffect(SFX::Just_Parring);
			SoundEffect::SetSoundEffectVolume(SFX::Just_Parring, SOUND);
			player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Parrying_Up));
		}
	}
}

void Parrying::Cooling(float dt)
{
	Skill::Cooling(dt);
	vec2 player_dir = player->Get_move_dir();
	if (player_dir == vec2(1, 1)) {
		direction = 1;
	}
	else if (player_dir == vec2(1, 0)) {
		direction = 2;
	}
	else if (player_dir == vec2(1, -1)) {
		direction = 3;
	}
	else if (player_dir == vec2(0, -1)) {
		direction = 4;
	}
	else if (player_dir == vec2(-1, -1)) {
		direction = 5;
	}
	else if (player_dir == vec2(-1, 0)) {
		direction = 6;
	}
	else if (player_dir == vec2(-1, 1)) {
		direction = 7;
	}
	else if (player_dir == vec2(0, 1)) {
		direction = 8;
	}

	if (now_delay() == false) {
		now_parrying = false;
	}
	effect->Update(dt);

}

bool Parrying::Get_now_parring() const
{
	return now_parrying;
}

void Parrying::determine_success(GameObject* object)
{
	if (now_delay() == true) {
		if (Engine::Instance()->GetGameStateManager().Get_current_state_name() == "Book3") {
			float for_calc = object->GetRotation();
			float standard = 0.1f;
			if (for_calc == 0) {
				if (direction == 8) {
					adjust_value = vec2(0, 30);
					success();
				}
			}
			//right
			else if (for_calc > standard) {
				if (direction == 2) {
					adjust_value = vec2(30, 0);
					success();
				}
			}
			//left
			else if (for_calc < standard) {
				if (direction == 6) {
					adjust_value = vec2(-30, 0);
					success();
				}
			}
		}
		else {
			vec2 for_calc = normalize(object->GetVelocity());
			if (for_calc.x <= 0 && for_calc.y <= 0) {
				if (direction == 1 || direction == 2 || direction == 8) {
					adjust_value = vec2(30, 0);
					success();
				}
			}
			else if (for_calc.x >= 0 && for_calc.y <= 0) {
				if (direction == 6 || direction == 7 || direction == 8) {
					adjust_value = vec2(-30, 0);
					success();
				}
			}
			else if (for_calc.x >= 0 && for_calc.y >= 0) {
				if (direction == 4 || direction == 5 || direction == 6) {
					adjust_value = vec2(0, -30);
					success();
				}
			}
			else if (for_calc.x <= 0 && for_calc.y >= 0) {
				if (direction == 2 || direction == 3 || direction == 4) {
					adjust_value = vec2(0, 30);
					success();
				}
			}
		}
	}

}

void Parrying::Draw(const mat3& world_to_ndc)
{
	const mat3 T = mat3::build_translation(player->GetPosition() + adjust_value);
	const mat3 H = mat3::build_scale(ui_size);

	mat3 final_matrix = world_to_ndc * T * H;
	effect->No_Gameobject_Draw(final_matrix);
}

void Parrying::success()
{
	effect->ResetAnimation();
	cooltimer.ZeroSet();
	now_parrying = true;
	SoundEffect::PlaySoundEffect(SFX::Parring);
	SoundEffect::SetSoundEffectVolume(SFX::Parring, SOUND);
}
