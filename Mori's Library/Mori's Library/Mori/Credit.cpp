#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "States.h"
#include "Credit.h"
#include "Background.h"

void Credit::Load()
{
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, { 0,0 }));
    AddGSComponent(new Background);

    GetGSComponent<Background>()->Add("assets/sprite/Credit.spt", 1, Engine::Instance()->Get_Window());
}

void Credit::Update([[maybe_unused]] float dt)
{
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }
}

void Credit::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Credit::Unload()
{
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}