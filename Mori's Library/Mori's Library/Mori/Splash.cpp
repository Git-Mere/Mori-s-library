#include "../Engine/Engine.h"
#include "States.h"
#include "Splash.h"
#include "../Engine/GameObjectManager.h"
#include "sample_logo.h"
#include "../Engine/CameraCom.h"
#include "../Engine/Font.h"

Splash::Splash(){
}

void Splash::Load() {
    glCheck(glClearColor(1, 1, 1, 1));
    counter = 0;
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({0,0}, Engine::Instance()->Get_iWindow()));
    GetGSComponent<GameObjectManager>()->Add(new Logo(vec2(0,0)));
}

void Splash::Draw() {
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Splash::Update([[maybe_unused]] float dt) {
    Engine::GetLogger().LogDebug(std::to_string(counter));
    if (counter >= 2) {
        Engine::GetGameStateManager().ClearNextGameState();
    }
    counter+= dt;

    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Splash::Unload() {
    Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
}