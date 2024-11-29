#include "book_scroll.h"
#include "portalbook_image.h"

Scroll::Scroll(vec2 position, std::string book_name, [[maybe_unused]] portalbook_image& portalbookscroll) : GameObject(hide_position), portalbook_image_(&portalbookscroll)
{
	AddGOComponent(new Sprite("assets/sprite/Scroll/" + book_name + ".spt", this));
	GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
	real_position = position;

	SetScale(vec2(475, 187.5f));
	current_state = &state_idle;
	current_state->Enter(this);

	GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
}

void Scroll::Update(float dt)
{
	GameObject::Update(dt);
}

void Scroll::Draw(const mat3& world_to_ndc)
{
	GameObject::Draw(world_to_ndc);
}

void Scroll::State_Idle::Enter([[maybe_unused]] GameObject* object) {
	Scroll* scroll = static_cast<Scroll*>(object);
	scroll->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
}

void Scroll::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
	Scroll* scroll = static_cast<Scroll*>(object);
	if (scroll->portalbook_image_->Get_is_open() == false && scroll->GetPosition() != scroll->hide_position) {
		scroll->SetPosition(scroll->hide_position);
	}

}

void Scroll::State_Idle::CheckExit([[maybe_unused]] GameObject* object) {
	Scroll* scroll = static_cast<Scroll*>(object);
	if (scroll->portalbook_image_->Get_is_open() == true) {
		scroll->SetPosition(scroll->real_position);
		scroll->change_state(&scroll->state_open);
	}
}



void Scroll::state_Open::Enter([[maybe_unused]] GameObject* object) {
	Scroll* scroll = static_cast<Scroll*>(object);
	scroll->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Open));
	timer = 0;
}

void Scroll::state_Open::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
	Scroll* scroll = static_cast<Scroll*>(object);
	if (scroll->portalbook_image_->Get_is_open() == false) {
		scroll->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::close));
		timer += dt;
	}
}

void Scroll::state_Open::CheckExit([[maybe_unused]] GameObject* object) {
	Scroll* scroll = static_cast<Scroll*>(object);
	if (scroll->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::close) && timer > 0.25f) {
		scroll->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
		scroll->SetPosition(scroll->hide_position);
		scroll->change_state(&scroll->state_idle);
	}
	if (scroll->portalbook_image_->Get_is_open() == true) {
		scroll->change_state(&scroll->state_open);
	}
}