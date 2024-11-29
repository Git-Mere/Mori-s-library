#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "Life.h"
#include "../Engine/Particels.h"
#include "FiterAttack.h"

namespace Particles {
//////////////////////////////////////// Energy Kick ////////////////////////////////////////
	Energy_Kick1::Energy_Kick1() : Particle("assets/sprite/Book4/Energy_Kick.spt") {
		SetScale(vec2(300, 300));
		SetCollisionScale(vec2(130, 200), true);
		SetPosition(vec2(-5000, -5000));
		current_state->Enter(this);
	};
	bool Energy_Kick1::CanCollideWith(GameObjectTypes other_object_type) {
		if (other_object_type == GameObjectTypes::Player) {
			return true;
		}
		return false;
	};
	Energy_Kick2::Energy_Kick2() : Particle("assets/sprite/Book4/Energy_Kick.spt") {
		SetScale(vec2(300, -300));
		SetCollisionScale(vec2(130, 200), true);
		SetPosition(vec2(-5000, -5000));
		current_state->Enter(this);
	};
	bool Energy_Kick2::CanCollideWith(GameObjectTypes other_object_type) {
		if (other_object_type == GameObjectTypes::Player) {
			return true;
		}
		return false;
	};
//////////////////////////////////////// Energy Fist ////////////////////////////////////////
	Energy_Fist::Energy_Fist() : Particle("assets/sprite/Book4/Energy_Fist.spt") {
		SetScale(vec2(-150, 150));
		SetCollisionScale(vec2(60, 60), true);
		SetPosition(vec2(-5000, -5000));
		current_state->Enter(this);
	};

	bool Energy_Fist::CanCollideWith(GameObjectTypes other_object_type) {
		if (other_object_type == GameObjectTypes::Player) {
			return true;
		}
		return false;
	}
	void Energy_Fist::ResolveCollision(GameObject* other_object)
	{
		Player* player = static_cast<Player*>(other_object);
		if (player->Get_parrying_mode()) {
			SetPosition(vec2(-5000, -5000));
		}
	}
	;

//////////////////////////////////////// Energy Lazer ////////////////////////////////////////
	Energy_Lazer_L::Energy_Lazer_L() : Particle("assets/sprite/Book4/Energy_Lazer.spt") {
		SetScale(vec2(1000, 210));
		SetCollisionScale(vec2(900, 100), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};

	bool Energy_Lazer_L::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player) {
				return true;
			}
		}
		return false;
	};

	//animation
	void Energy_Lazer_L::State_Ready::Enter(GameObject* object) {
		Energy_Lazer_L* lazer = static_cast<Energy_Lazer_L*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		lazer->SetCollision(false);
	}

	void Energy_Lazer_L::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Energy_Lazer_L* lazer = static_cast<Energy_Lazer_L*>(object);
		if (lazer->Alive()) {
			count1 += dt;
		}
	}

	void Energy_Lazer_L::State_Ready::CheckExit(GameObject* object) {
		if (count1 > ready_time) {
			Energy_Lazer_L* lazer = static_cast<Energy_Lazer_L*>(object);
			lazer->change_state(&lazer->state_shoot);
			count1 = 0;
		}
	}

	void Energy_Lazer_L::State_Shoot::Enter(GameObject* object) {
		Energy_Lazer_L* lazer = static_cast<Energy_Lazer_L*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		lazer->SetCollision(true);
	}

	void Energy_Lazer_L::State_Shoot::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Energy_Lazer_L* lazer = static_cast<Energy_Lazer_L*>(object);
		if (lazer->Alive()) {
			count2 += dt;
		}
	}

	void Energy_Lazer_L::State_Shoot::CheckExit([[maybe_unused]] GameObject* object) {
		Energy_Lazer_L* lazer = static_cast<Energy_Lazer_L*>(object);
		if (!lazer->Alive()) {
			lazer->change_state(&lazer->state_ready);
			count2 = 0;
		}
	}
	//R
	Energy_Lazer_R::Energy_Lazer_R() : Particle("assets/sprite/Book4/Energy_Lazer.spt") {
		SetScale(vec2(-1000, 210));
		SetCollisionScale(vec2(900, 100), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};

	bool Energy_Lazer_R::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player) {
				return true;
			}
		}
		return false;
	};

	//animation
	void Energy_Lazer_R::State_Ready::Enter(GameObject* object) {
		Energy_Lazer_R* lazer = static_cast<Energy_Lazer_R*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		lazer->SetCollision(false);
	}

	void Energy_Lazer_R::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Energy_Lazer_R* lazer = static_cast<Energy_Lazer_R*>(object);
		if (lazer->Alive()) {
			count1 += dt;
		}
	}

	void Energy_Lazer_R::State_Ready::CheckExit(GameObject* object) {
		if (count1 > ready_time) {
			Energy_Lazer_R* lazer = static_cast<Energy_Lazer_R*>(object);
			lazer->change_state(&lazer->state_shoot);
			count1 = 0;
		}
	}

	void Energy_Lazer_R::State_Shoot::Enter(GameObject* object) {
		Energy_Lazer_R* lazer = static_cast<Energy_Lazer_R*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		lazer->SetCollision(true);
	}

	void Energy_Lazer_R::State_Shoot::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		Energy_Lazer_R* lazer = static_cast<Energy_Lazer_R*>(object);
		if (lazer->Alive()) {
			count2 += dt;
		}
	}

	void Energy_Lazer_R::State_Shoot::CheckExit([[maybe_unused]] GameObject* object) {
		Energy_Lazer_R* lazer = static_cast<Energy_Lazer_R*>(object);
		if (!lazer->Alive()) {
			lazer->change_state(&lazer->state_ready);
			count2 = 0;
		}
	}

//////////////////////////////////////// PunchEarthquake ////////////////////////////////////////
	PunchEarthquake::PunchEarthquake() : Particle("assets/sprite/Book4/Earthquake.spt") {
		SetScale(vec2(300, 300));
		SetCollisionScale(vec2(70, 110), true);
		SetPosition(vec2(-5000, -5000));
		current_state->Enter(this);
	};
	bool PunchEarthquake::CanCollideWith(GameObjectTypes other_object_type) {
		if (other_object_type == GameObjectTypes::Player) {
			return true;
		}
		return false;
	};

//////////////////////////////////////// Energy Lazer(Big) ////////////////////////////////////////
	B_Energy_Lazer_L::B_Energy_Lazer_L() : Particle("assets/sprite/Book4/Energy_Lazer.spt") {
		SetScale(vec2(1000, 630));
		SetCollisionScale(vec2(900, 300), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};

	bool B_Energy_Lazer_L::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player) {
				return true;
			}
		}
		return false;
	};

	//animation
	void B_Energy_Lazer_L::State_Ready::Enter(GameObject* object) {
		B_Energy_Lazer_L* lazer = static_cast<B_Energy_Lazer_L*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		lazer->SetCollision(false);
	}

	void B_Energy_Lazer_L::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		B_Energy_Lazer_L* lazer = static_cast<B_Energy_Lazer_L*>(object);
		if (lazer->Alive()) {
			count1 += dt;
		}
	}

	void B_Energy_Lazer_L::State_Ready::CheckExit(GameObject* object) {
		if (count1 > ready_time) {
			B_Energy_Lazer_L* lazer = static_cast<B_Energy_Lazer_L*>(object);
			lazer->change_state(&lazer->state_shoot);
			count1 = 0;
		}
	}

	void B_Energy_Lazer_L::State_Shoot::Enter(GameObject* object) {
		B_Energy_Lazer_L* lazer = static_cast<B_Energy_Lazer_L*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		lazer->SetCollision(true);
	}

	void B_Energy_Lazer_L::State_Shoot::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		B_Energy_Lazer_L* lazer = static_cast<B_Energy_Lazer_L*>(object);
		if (lazer->Alive()) {
			count2 += dt;
		}
	}

	void B_Energy_Lazer_L::State_Shoot::CheckExit([[maybe_unused]] GameObject* object) {
		B_Energy_Lazer_L* lazer = static_cast<B_Energy_Lazer_L*>(object);
		if (!lazer->Alive()) {
			lazer->change_state(&lazer->state_ready);
			count2 = 0;
		}
	}
	//R
	B_Energy_Lazer_R::B_Energy_Lazer_R() : Particle("assets/sprite/Book4/Energy_Lazer.spt") {
		SetScale(vec2(-1000, 630));
		SetCollisionScale(vec2(900, 300), true);
		SetPosition(vec2(-5000, -5000));
		current_state = &state_ready;
		current_state->Enter(this);
	};

	bool B_Energy_Lazer_R::CanCollideWith(GameObjectTypes other_object_type) {
		if (is_collision == true) {
			if (other_object_type == GameObjectTypes::Player) {
				return true;
			}
		}
		return false;
	};

	//animation
	void B_Energy_Lazer_R::State_Ready::Enter(GameObject* object) {
		B_Energy_Lazer_R* lazer = static_cast<B_Energy_Lazer_R*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
		lazer->SetCollision(false);
	}

	void B_Energy_Lazer_R::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		B_Energy_Lazer_R* lazer = static_cast<B_Energy_Lazer_R*>(object);
		if (lazer->Alive()) {
			count1 += dt;
		}
	}

	void B_Energy_Lazer_R::State_Ready::CheckExit(GameObject* object) {
		if (count1 > ready_time) {
			B_Energy_Lazer_R* lazer = static_cast<B_Energy_Lazer_R*>(object);
			lazer->change_state(&lazer->state_shoot);
			count1 = 0;
		}
	}

	void B_Energy_Lazer_R::State_Shoot::Enter(GameObject* object) {
		B_Energy_Lazer_R* lazer = static_cast<B_Energy_Lazer_R*>(object);
		lazer->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Boom));
		lazer->SetCollision(true);
	}

	void B_Energy_Lazer_R::State_Shoot::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
		B_Energy_Lazer_R* lazer = static_cast<B_Energy_Lazer_R*>(object);
		if (lazer->Alive()) {
			count2 += dt;
		}
	}

	void B_Energy_Lazer_R::State_Shoot::CheckExit([[maybe_unused]] GameObject* object) {
		B_Energy_Lazer_R* lazer = static_cast<B_Energy_Lazer_R*>(object);
		if (!lazer->Alive()) {
			lazer->change_state(&lazer->state_ready);
			count2 = 0;
		}
	}

}