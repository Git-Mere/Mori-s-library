#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "States.h"
#include "Rolling_Credit.h"
#include "Background.h"


void Rolling_Credit::Load() {
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, { 0,0 }));
    AddGSComponent(new Background);
    
    GetGSComponent<Background>()->Add("assets/sprite/Rolling_Credit/Press.spt", 1, Engine::Instance()->Get_Window());
    
    vec2 windowSize = Engine::Instance()->Get_Window();

    std::vector<std::string> spritePaths = {
        "assets/sprite/Rolling_Credit/Rolling_Credit1.spt",
        "assets/sprite/Rolling_Credit/Rolling_Credit2.spt",
        "assets/sprite/Rolling_Credit/Rolling_Credit3.spt",
        "assets/sprite/Rolling_Credit/Rolling_Credit4.spt",
        "assets/sprite/Rolling_Credit/Rolling_Credit5.spt",
        "assets/sprite/Rolling_Credit/Rolling_Credit6.spt"
    };

    float initialY = 0.0f;
    float spacing = windowSize.y -1.5f; 
    float lastSpacing = windowSize.y - 28.0f;
    
    for (size_t i = 0; i < spritePaths.size(); ++i) {
        float currentSpacing = (i == spritePaths.size() - 1) ? lastSpacing : spacing;
        vec2 position = vec2(0.0f, initialY - (i * currentSpacing));
        GetGSComponent<GameObjectManager>()->Add(new Credits(spritePaths[i], position));
    }
}

void Rolling_Credit::Update([[maybe_unused]] float dt) {
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }
}

void Rolling_Credit::Draw() {
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Rolling_Credit::Unload() {
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}


Credits::Credits(const std::string& spritePath, vec2 position) {
    AddGOComponent(new Sprite(spritePath, this));
    SetScale(vec2(1280, 720));
    SetPosition(position);
}

void Credits::Update(float dt) {
    if (Engine::GetInput().KeyDown(Input::Keys::Down)){
        rolling_speed = 400.0f;
    }
    else {
		rolling_speed = 100.0f;
	}
    UpdatePosition(vec2(0, rolling_speed * dt));
}
