#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "Option.h"
#include "States.h"
#include "Background.h"
#include "Soundeffect.h"
const float sound_levels[] = { 0.0f, 25.0f, 50.0f, 75.0f, 100.0f };

void Option::Load() {
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, { 0,0 }));
    AddGSComponent(new Background);

    GetGSComponent<Background>()->Add("assets/sprite/Option.spt", 1, Engine::Instance()->Get_Window());

    vec2 window_size = Engine::Instance()->Get_Window();
   
    if (SOUND == sound_levels[0]) {
        select = 0;
    }
    else if (SOUND == sound_levels[1]) {
        select = 1;
    }
    else if (SOUND == sound_levels[2]) {
        select = 2;
    }
    else if (SOUND == sound_levels[3]) {
        select = 3;
    }
    else if (SOUND == sound_levels[4]) {
        select = 4;
    }

    float start_x = -window_size.x / 2 + 300;
    float range_width = 150.0f * (total_select - 1);
    cursor_ptr = new Horizontal_Cursor(vec2(start_x - 100, -100), { {start_x - 100, 0}, {start_x - 100 + range_width, window_size.y / 3} }, total_select);
    GetGSComponent<GameObjectManager>()->Add(cursor_ptr);
    cursor_ptr->move_pos(select);
}

void Option::Update([[maybe_unused]] float dt) {
    int how = 1;
    if (Engine::GetInput().KeyJustReleased(Input::Keys::Right)) {
        if (select + how < total_select) {
            select += how;
        }
        cursor_ptr->move_pos(select);
    }
    else if (Engine::GetInput().KeyJustReleased(Input::Keys::Left)) {
        if (select - how >= 0) {
            select -= how;
        }
        cursor_ptr->move_pos(select);
        
    }
    if (Engine::GetInput().KeyJustReleased(Input::Keys::Enter)) {
        SOUND = sound_levels[select];
        SoundEffect::PlaySoundEffect(SFX::Good_answer);
        SoundEffect::SetSoundEffectVolume(SFX::Good_answer,SOUND);
    }

    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }
}

void Option::Draw() {
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Option::Unload() {
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}
