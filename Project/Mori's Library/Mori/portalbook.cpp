#include "portalbook.h"
#include "States.h"
#include "../Engine/GameObjectManager.h"
#include "../ScreenEffect.h"

portalbook::portalbook(vec2 position, int what_state, std::string book_name, Engine::ShaderName shader_name) : to_state(what_state),
GameObject(position), file_name(book_name), recation_shader_name(shader_name)
{
	AddGOComponent(new DownTimer(3));
	if (file_name == "eddy") {
		AddGOComponent(new Sprite("assets/sprite/Portalbook/eddy.spt", this));
		SetScale(vec2(50));
	}
	else if (file_name == "Hole") {
		SetScale(vec2(400));
		SetCollisionScale(vec2(200), true);
		AddGOComponent(new Sprite("assets/sprite/Portalbook/Hole.spt", this));
	}
	else{
		AddGOComponent(new Sprite("assets/sprite/Portalbook/portalbook.spt", this));
		SetShaderName(Engine::noimage);
		SetScale(vec2(50));
	}

	if (file_name != "portalbook" || file_name != "eddy") {
		image_ptr = new portalbook_image(position, file_name);
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(image_ptr);
	}
}

void portalbook::Update(float dt)
{
	GameObject::Update(dt);
	if (one_coliision == false) {
		if (GetGOComponent<DownTimer>()->end()) {
			go_state();
		}
	}
}

bool portalbook::CanCollideWith(GameObjectTypes other_object_type)
{
	if (other_object_type == GameObjectTypes::Player && one_coliision == true) {
		return true;
	}
	return false;
}

void portalbook::ResolveCollision(GameObject* other_object)
{
	if (other_object->Type() == GameObjectTypes::Player) {
		if (file_name == "eddy") {
			Engine::Instance()->GetGameStateManager().GetGSComponent<UIManager>()->SE_setting(recation_shader_name, 5);
			Player* user = static_cast<Player*>(other_object);
			if (user->Get_what_state() == States::Tutorial) {
				if (user->GetPosition().x < 0) {
					Engine::Instance()->Set_Gamemode(Engine::Easy);
				}
				else {
					Engine::Instance()->Set_Gamemode(Engine::Hard);
				}
			}
			Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(jump_waiting, 5);
		}
		else {
			if (file_name == "Exit") {
				Engine::Instance()->GetGameStateManager().GetGSComponent<UIManager>()->SE_setting(recation_shader_name, 5, false);
			}
			else if (file_name == "Exit2") {
				Engine::Instance()->GetGameStateManager().GetGSComponent<UIManager>()->SE_setting(recation_shader_name, 5);
			}
			else {
				Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(jump_waiting, 5);
				Engine::Instance()->GetGameStateManager().GetGSComponent<UIManager>()->SE_setting(recation_shader_name, 5);
			}

		}
		GetGOComponent<DownTimer>()->Set(jump_waiting);
		one_coliision = false;
	}

}

void portalbook::ChangePosition(vec2 position_) {
	SetPosition(position_);
	image_ptr->ChangePosition(position_);
}


void portalbook::go_state()
{
	SoundEffect::EraseSoundEffect();
	Engine::GetGameStateManager().SetNextGameState(to_state);
}