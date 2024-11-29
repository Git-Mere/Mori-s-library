#include "tuto_dummy.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "Mori/Player.h"
#include "Engine/Skill.h"
#include "Engine/SkillManager.h"
#include "Tuto_ball.h"
#include "Mori/FontObject.h"

tuto_dummy::tuto_dummy(vec2 position, vec2 scale, Player& p_ptr, float rotation) : GameObject(position, rotation, scale), start_position(position), player_(&p_ptr)
{
	AddGOComponent(new Sprite("assets/sprite/Tuto/scarecrow.spt", this));
	explain1 = new FontObject(vec2(0, 0), " ");
	clear_count_font = new FontObject(vec2(0, -150), "0 / 5");
	clear_count_font->SetFontSize(vec2(100, 100));

	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(explain1);
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(clear_count_font);

	invin = new Invincible;
	SetCollisionScale(vec2(70, 100), true);
	AddGOComponent(invin);
	current_state = &state_sandbag;
	current_state->Enter(this);
	mode = Sandbag;
}



void tuto_dummy::Draw(const mat3& world_to_ndc)
{
	if (invin->is_hurt() == false || invin->TickTock() == true) {
		GameObject::Draw(world_to_ndc);
	}
}

bool tuto_dummy::CanCollideWith(GameObjectTypes other_object_type)
{
	if (other_object_type == GameObjectTypes::Player) {
		return true;
	}
	return false;
}

void tuto_dummy::ResolveCollision(GameObject* other_object)
{
	if (invin->is_invin() == false && player_->GetGOComponent<SkillManager>()->GetComponent<dash>()->now_delay() == true) {
		invin->Set_invin(hurt_time, true);
		float x = (other_object->GetPosition().x - GetPosition().x) * 2;
		float y = (other_object->GetPosition().y - GetPosition().y) * 2;
		vec2 move_direction = { x , y };
		move_direction = normalize(move_direction);
		move_direction *= -300;
		SetVelocity(move_direction);
		state_sandbag.clear_count++;
	}
}

void tuto_dummy::count()
{
	if (mode == Dodge) {
		state_dodge.clear_count++;
	}
	else if (mode == Parrying) {
		state_parrying.clear_count++;
	}
}

void tuto_dummy::State_Sandbag::Enter([[maybe_unused]]GameObject* object)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	dummy->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
}

void tuto_dummy::State_Sandbag::Update(GameObject* object, [[maybe_unused]] float dt)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	dummy->clear_count_font->change_text(std::to_string(clear_count) + " / 5");
	if (dummy->invin->is_invin() == false) {
		dummy->SetPosition(dummy->start_position);
		dummy->SetVelocity({0,0});
		if (clear_count >= clear_) {
			dummy->mode_clear = true;
		}
	}
}

void tuto_dummy::State_Sandbag::CheckExit(GameObject* object)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	if (dummy->mode_clear) {
		dummy->change_state(&dummy->state_dodge);
	}
}


void tuto_dummy::State_Dodge::Enter(GameObject* object)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	dummy->mode = Dodge;
	dummy->mode_clear = false;
}

void tuto_dummy::State_Dodge::Update(GameObject* object, [[maybe_unused]] float dt)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	dummy->SetPosition(dummy->start_position);
	dummy->SetVelocity({ 0,0 });
	dummy->clear_count_font->change_text(std::to_string(clear_count) + " / 3");
	if (dummy->GetGOComponent<DownTimer>()->end()) {
		dummy->GetGOComponent<DownTimer>()->Set(javeline_frequency);
		dummy->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Throw));
	}
	else {
		if (dummy->GetGOComponent<Sprite>()->AnimationEnded()) {
			vec2 move_direction = normalize(dummy->player_->GetPosition() - dummy->GetPosition());
			move_direction *= javeline_speed;
			Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Tuto_ball>>()->Shot(1, vec2(dummy->GetPosition().x + 15, dummy->GetPosition().y - 50), move_direction);
			dummy->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
		}
	}
	if (clear_count >= clear_) {
		dummy->mode_clear = true;
	}
}

void tuto_dummy::State_Dodge::CheckExit(GameObject* object)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	if (dummy->mode_clear) {
		dummy->change_state(&dummy->state_parrying);


	}
}

void tuto_dummy::State_Parrying::Enter(GameObject* object)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	dummy->mode = Parrying;
	dummy->mode_clear = false;

}

void tuto_dummy::State_Parrying::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	dummy->SetPosition(dummy->start_position);
	dummy->SetVelocity({ 0,0 });
	dummy->clear_count_font->change_text(std::to_string(clear_count) + " / 3");
;	if (dummy->GetGOComponent<DownTimer>()->end()) {
		dummy->GetGOComponent<DownTimer>()->Set(javeline_frequency);
		dummy->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Throw));
	}
	else {
		if (dummy->GetGOComponent<Sprite>()->AnimationEnded()) {
			vec2 move_direction = normalize(dummy->player_->GetPosition() - dummy->GetPosition());
			move_direction *= javeline_speed;
			Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Tuto_ball>>()->Shot(1, vec2(dummy->GetPosition().x + 15 , dummy->GetPosition().y - 50), move_direction);
			dummy->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
		}
	}
	if (clear_count >= clear_) {
		dummy->mode_clear = true;
	}
}

void tuto_dummy::State_Parrying::CheckExit(GameObject* object)
{
	tuto_dummy* dummy = static_cast<tuto_dummy*>(object);
	if (dummy->mode_clear) {
		dummy->player_->SetPosition(vec2(0, -300));

		dummy->explain1->change_text("Easy Mode");
		dummy->explain1->SetFontPosition(vec2(-200, -100));
		dummy->explain1->SetFontSize(vec2(200, 100));

		dummy->clear_count_font->change_text("Hard Mode");
		dummy->clear_count_font->SetFontPosition(vec2(200, -100));
		dummy->clear_count_font->SetFontSize(vec2(200, 100));

		dummy->mode = End;
	}

}


