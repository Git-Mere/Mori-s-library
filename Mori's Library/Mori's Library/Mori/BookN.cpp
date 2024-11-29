#include "BookN.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "States.h"
#include "Player.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "FontObject.h"
#include "../Engine/Gravity.h"
#include "ground.h"


void BookN::Load()
{
	AddGSComponent(new GameObjectManager);
	glCheck(glClearColor(0.807f, 0.835f, 0.898f, 1.0f));
	AddGSComponent(new CameraComponent({ 0,0 }, { 0,0 }));
	AddGSComponent(new ShowCollision);
	AddGSComponent(new Gravity(800));


	player_ptr = new Player({ 0, 0 }, vec2(50));
	//player_ptr->change_player_scale(vec2(100));
	//say_object = new FontObject(vec2(300, 200), capatin_say.getSentences(attack->GetIndex()));
	//say_object->SetFontSize(vec2(300, 200));

	//previous_index = attack->GetIndex();
	vec2 window_size = Engine::Instance()->Get_Window();
	GetGSComponent<GameObjectManager>()->Add(new ground(vec2(0, -window_size.y / 2), vec2(window_size.x, 50)));
	GetGSComponent<GameObjectManager>()->Add(new ground(vec2(200, -window_size.y / 2), vec2(200, 200)));
	GetGSComponent<GameObjectManager>()->Add(new ground(vec2(-200, -window_size.y / 2), vec2(200, 200)));

	GetGSComponent<GameObjectManager>()->Add(player_ptr);
	//GetGSComponent<GameObjectManager>()->Add(say_object);
}

void BookN::Update(float dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
	GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());

	if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
	}
}

void BookN::Draw()
{
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}


void BookN::Unload()
{
	GetGSComponent<GameObjectManager>()->Unload();
	ClearGSComponents();
}