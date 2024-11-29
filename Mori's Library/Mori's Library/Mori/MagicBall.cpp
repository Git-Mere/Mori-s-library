#include "MagicBall.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include <Random.h>
#include "Life.h"

namespace Particles {
	MagicBall::MagicBall() : Particle("assets/sprite/Book2/Sand_Pit.spt") {
		SetScale(vec2(230));
		SetCollisionScale(vec2(160), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};
	bool MagicBall::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player ) {
				return true;
			}
		}
		return false;
	};

	void MagicBall::State_Ready::Enter(GameObject* object) {
		MagicBall* magic_ball = static_cast<MagicBall*>(object);
		magic_ball->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		magic_ball->SetCollision(false);
	}

	void MagicBall::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		MagicBall* magic_ball = static_cast<MagicBall*>(object);
		if (magic_ball->Alive()) {
			count1 += dt;
		}
	}

	void MagicBall::State_Ready::CheckExit(GameObject* object) {
		if (count1 > ready_time) {
			MagicBall* magic_ball = static_cast<MagicBall*>(object);
			magic_ball->change_state(&magic_ball->state_boom);
			count1 = 0;
		}
	}

	void MagicBall::State_Boom::Enter(GameObject* object) {
		MagicBall* magic_ball = static_cast<MagicBall*>(object);
		magic_ball->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		magic_ball->SetCollision(true);
	}

	void MagicBall::State_Boom::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		MagicBall* magic_ball = static_cast<MagicBall*>(object);
		if (magic_ball->Alive()) {
			count2 += dt;
		}
	}

	void MagicBall::State_Boom::CheckExit([[maybe_unused]] GameObject* object) {
		MagicBall* magic_ball = static_cast<MagicBall*>(object);
		if (!magic_ball->Alive()) {
			magic_ball->change_state(&magic_ball->state_ready);
			count2 = 0;
		}
	}

	////////////////////////////////////////////////////////////////
	Earthquake::Earthquake() : Particle("assets/sprite/Book2/Earthquake.spt") {
		SetScale(vec2(500 , 1000));
		SetCollisionScale(vec2(300, 300), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};
	bool Earthquake::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player) {
				return true;
			}
		}
		return false;
	};

	void Earthquake::State_Ready::Enter(GameObject* object) {
		Earthquake* earthquake = static_cast<Earthquake*>(object);
		earthquake->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		earthquake->SetCollision(false);
	}

	void Earthquake::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Earthquake* earthquake = static_cast<Earthquake*>(object);
		if (earthquake->Alive()) {
			count1 += dt;
		}
	}

	void Earthquake::State_Ready::CheckExit([[maybe_unused]]GameObject* object) {
		Earthquake* earthquake = static_cast<Earthquake*>(object);
		if (earthquake->GetGOComponent<Sprite>()->get_frame().x == 6) {
			count1 = 0;
			earthquake->change_state(&earthquake->state_boom);
		}
	}

	void Earthquake::State_Boom::Enter(GameObject* object) {
		Earthquake* earthquake = static_cast<Earthquake*>(object);
		earthquake->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		earthquake->SetCollision(true);
	}

	void Earthquake::State_Boom::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Earthquake* earthquake = static_cast<Earthquake*>(object);
		if (earthquake->Alive()) {
			count2 += dt;
		}
	}

	void Earthquake::State_Boom::CheckExit([[maybe_unused]] GameObject* object) {
		Earthquake* earthquake = static_cast<Earthquake*>(object);
		if (!earthquake->Alive()) {
			count2 = 0;
			earthquake->change_state(&earthquake->state_ready);
		}
	}
}