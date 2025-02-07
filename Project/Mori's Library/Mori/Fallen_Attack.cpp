#include "Fallen_Attack.h"
#include "../Engine/Particels.h"

namespace Particles {
	Fire_Pillar::Fire_Pillar() : Particle("assets/sprite/Bible/Fire_Pillar.spt") {
		SetScale(vec2(50, Engine::Instance()->Get_Window().y));
		SetCollisionScale(vec2(40, Engine::Instance()->Get_Window().y), true);
		SetPosition(vec2(-5000, -5000));
		current_state->Enter(this);
	};
	bool Fire_Pillar::CanCollideWith(GameObjectTypes other_object_type) {
		if (other_object_type == GameObjectTypes::Player) {
			return true;
		}
		return false;
	};

	/////////////////////////////////////////////////////////////////
	Fire_Pit::Fire_Pit() : Particle("assets/sprite/Bible/Fire_Pit.spt") {
		SetScale(vec2(230));
		SetCollisionScale(vec2(100), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};
	
	bool Fire_Pit::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player) {
				return true;
			}
		}
		return false;
	};

	void Fire_Pit::State_Ready::Enter(GameObject* object) {
		Fire_Pit* fire_pit = static_cast<Fire_Pit*>(object);
		fire_pit->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		fire_pit->SetCollision(false);
	}

	void Fire_Pit::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Fire_Pit* fire_pit = static_cast<Fire_Pit*>(object);
		if (fire_pit->Alive()) {
			count1 += dt;
		}
	}

	void Fire_Pit::State_Ready::CheckExit(GameObject* object) {
		if (count1 > ready_time) {
			Fire_Pit* fire_pit = static_cast<Fire_Pit*>(object);
			fire_pit->change_state(&fire_pit->state_boom);
			count1 = 0;
		}
	}

	void Fire_Pit::State_Boom::Enter(GameObject* object) {
		Fire_Pit* fire_pit = static_cast<Fire_Pit*>(object);
		fire_pit->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		fire_pit->SetCollision(true);
	}

	void Fire_Pit::State_Boom::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Fire_Pit* fire_pit = static_cast<Fire_Pit*>(object);
		if (fire_pit->Alive()) {
			count2 += dt;
		}
	}

	void Fire_Pit::State_Boom::CheckExit([[maybe_unused]] GameObject* object) {
		Fire_Pit* fire_pit = static_cast<Fire_Pit*>(object);
		if (!fire_pit->Alive()) {
			fire_pit->change_state(&fire_pit->state_ready);
			count2 = 0;
		}
	}

//////////////////////////////////////


}