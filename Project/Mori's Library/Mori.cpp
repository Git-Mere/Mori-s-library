#include "Mori.h"
#include "../Engine/Collision.h"
#include <iostream>
#include "Engine/GameObjectManager.h"
#include "tuto_dummy.h"
#include <Random.h>


Mori::Mori(vec2 position, vec2 scale) : GameObject(position, 0, scale)
{
	std::string tmp = Engine::GetGameStateManager().Get_current_state_name();

	if (tmp == "libarary") {
		AddGOComponent(new Sprite("assets/sprite/Mori/Mori_sit.spt", this));
		SetCollisionScale({ scale.x * 0.8f, scale.y * 0.7f }, true);
	}
	else{
		AddGOComponent(new Sprite("assets/sprite/Mori/Mori_stand.spt", this));
		SetCollisionScale({ scale.x * 0.4f, scale.y * 0.7f }, true);
	}

}


Mori_talk::Mori_talk(Mori& value, Player& player) : GameObject(), mori(&value), player(&player)
{
	SetPosition(mori->GetPosition());
	std::string tmp = Engine::GetGameStateManager().Get_current_state_name();
	if (tmp == "libarary") {
		SetTranslation_Pivot(vec2(180, 100));
		SetScale(vec2(270, 225));
		what_state = States::Libarary;
		AddGOComponent(new Sprite("assets/sprite/Mori/library/library_say.spt", this));//

		SetCollisionScale(mori->GetCollisionScale() + vec2(30, 100), true);
	}
	else if(tmp == "Tutorial") {
		SetTranslation_Pivot(vec2(150, 100));
		SetScale(vec2(300, 250));
		what_state = States::Tutorial;
		AddGOComponent(new Sprite("assets/sprite/Mori/tuto/tuto_say.spt", this));

		dummy = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetObject<tuto_dummy>();
	}
	else if (tmp == "basement") {
		SetTranslation_Pivot(vec2(150, 100));
		SetScale(vec2(300, 250));
		what_state = States::Basement;
		AddGOComponent(new Sprite("assets/sprite/Mori/basement/basement_say.spt", this));
		GetGOComponent<Sprite>()->PlayAnimation(Start);
		change_state(&state_say);
		control_helper = false;
	}
	else if (tmp == "Bible") {
		SetTranslation_Pivot(vec2(150, 100));
		SetScale(vec2(300, 250));
		what_state = States::Bible;
		AddGOComponent(new Sprite("assets/sprite/Mori/bible/bible_say.spt", this));
		GetGOComponent<Sprite>()->PlayAnimation(Start);
		change_state(&state_say);
		control_helper = false;
	}
	limit = GetGOComponent<Sprite>()->get_animation_num();
}

void Mori_talk::Update(float dt)
{
	GameObject::Update(dt);
	collision_on = GetGOComponent<RectCollision>()->IsCollidingWith(player);
}

bool Mori_talk::CanCollideWith(GameObjectTypes other_object_type)
{
	if (mori->no_mori == false && control_helper == true && other_object_type == GameObjectTypes::Player) {
		return true;
	}
	return false;
}

void Mori_talk::ResolveCollision([[maybe_unused]]GameObject* other_object)
{
	if (control_helper == true) {
		change_state(&state_open);
		control_helper = false;
	}
}

bool Mori_talk::tuto_animation_end()
{
	//because of none.anm
	if (starting_animation == limit - 1) {
		return true;
	}
	return false;
}

void Mori_talk::first_tuto()
{
	dummy = new tuto_dummy(vec2{ 20, 0 }, vec2(300), *player);
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(dummy);
	player->SetPosition(vec2(0, -300));
	mori->SetPosition(vec2(300, 0));
	SetPosition(mori->GetPosition());
}

void Mori_talk::second_tuto()
{
	mori->SetPosition(vec2(0, 100));
	SetPosition(mori->GetPosition());
}


void Mori_talk::State_Say::Enter(GameObject* object)
{
	Mori_talk* talk = static_cast<Mori_talk*>(object);
	talk->GetGOComponent<Sprite>()->PlayAnimation(talk->starting_animation);
	if (talk->what_state == States::Bible || talk->what_state == States::Basement) {
		lock = true;
	}
}//

void Mori_talk::State_Say::Update(GameObject* object, [[maybe_unused]] float dt)
{
	Mori_talk* talk = static_cast<Mori_talk*>(object);
	if (talk->what_state == States::Tutorial) {
		if (lock == false && talk->starting_animation <= 5) {
			if (Engine::GetInput().KeyJustReleased(Input::Keys::G)) {
				talk->starting_animation++;
				if (talk->starting_animation == 6) {
					lock = true;
					talk->first_tuto();
				}
				next = true;
			}
		}
		else {
			if (tuto_mode != Mode::End && tuto_mode != talk->dummy->Get_mode()) {
				tuto_mode = talk->dummy->Get_mode();
				talk->starting_animation++;
				next = true;
			}
			else if(tuto_mode == End) {
				talk->second_tuto();
			}
		}

	}
	else if (talk->what_state == States::Bible || talk->what_state == States::Basement) {
		if (Engine::GetInput().KeyJustReleased(Input::Keys::G)) {
			if (talk->starting_animation <= talk->limit) {
				talk->starting_animation++;
				next = true;
			}
		}
	}
	else {
		if (Engine::GetInput().KeyJustReleased(Input::Keys::G)) {
			talk->starting_animation = util::random(Start, talk->limit);
			if (pre_ani == talk->starting_animation) {
				if (talk->starting_animation == talk->limit) {
					talk->starting_animation--;
				}
				else {
					talk->starting_animation++;
				}
			}

			pre_ani = talk->starting_animation;
			next = true;
		}
	}
}

void Mori_talk::State_Say::CheckExit(GameObject* object)
{
	Mori_talk* talk = static_cast<Mori_talk*>(object);
	if (talk->collision_on == false && lock != true) {
		talk->change_state(&talk->state_close);
	}
	if (next) {
		talk->change_state(&talk->state_say);
		next = false;
	}
}


void Mori_talk::State_Open::Enter(GameObject* object)
{
	Mori_talk* talk = static_cast<Mori_talk*>(object);
	talk->GetGOComponent<Sprite>()->PlayAnimation(Animations::Open);
}

void Mori_talk::State_Open::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt)
{
}

void Mori_talk::State_Open::CheckExit(GameObject* object)
{
	Mori_talk* talk = static_cast<Mori_talk*>(object);
	if (talk->GetGOComponent<Sprite>()->AnimationEnded()) {
		if (talk->collision_on == true) {
			talk->change_state(&talk->state_say);
		}
		else if(talk->collision_on == false) {
			talk->change_state(&talk->state_close);
		}
	}
}




void Mori_talk::State_Close::Enter(GameObject* object)
{
	Mori_talk* talk = static_cast<Mori_talk*>(object);
	talk->GetGOComponent<Sprite>()->PlayAnimation(Animations::Close);
	talk->control_helper = true;
}

void Mori_talk::State_Close::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt)
{
}

void Mori_talk::State_Close::CheckExit([[maybe_unused]] GameObject* object)
{
}


