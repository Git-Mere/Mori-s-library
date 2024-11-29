/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Mainmenu.cpp
Project:     Engine
Author:     Seungheon Jeon
Created:    March 8, 2023
Updated:    May 31, 2023
*/
extern float SOUND;

#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "Mainmenu.h"
#include "States.h"
#include "Cursor.h"
#include "Book2.h"
#include "Background.h"

void Mainmenu::Load()
{
    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::MainmenuBGM);
        SoundEffect::Loop_SoundEffect(SFX::MainmenuBGM);
        SoundEffect::SetSoundEffectVolume(SFX::MainmenuBGM, SOUND);
        SoundFlag = true;
    }
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, { 0,0 }));
    AddGSComponent(new Background);

    GetGSComponent<Background>()->Add("assets/sprite/mainmenu.spt", 1, Engine::Instance()->Get_Window());

    vec2 a = Engine::Instance()->Get_Window();
    cursor_ptr = new Cursor(vec2(a.x / 4, -a.y / 24), { {0,0},{a.x / 12.8f, -a.y / 1.75f} }, total_select);//
    //cursor_ptr = new Cursor(vec2(270, -30), { {0,0},{100,-400} }, total_select);//
    GetGSComponent<GameObjectManager>()->Add(cursor_ptr);
    select = 0;
}

void Mainmenu::Update([[maybe_unused]] float dt)
{
    int how = 1;
    if (Engine::GetInput().KeyJustReleased(Input::Keys::Down)) {
        if (select + how < total_select) {
            select += how;
        }
        cursor_ptr->move_pos(select);
    }
    else if (Engine::GetInput().KeyJustReleased(Input::Keys::Up)) {
        if (select - how >= 0) {
            select -= how;
        }
        cursor_ptr->move_pos(select);
    }
    else if (Engine::GetInput().KeyJustReleased(Input::Keys::Enter)) {
        if (select == 0) {
            if (first == false) {
                first = true;
                SoundEffect::EraseSoundEffect();
                Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Corridor));
            }
            else {
                SoundEffect::EraseSoundEffect();
                Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Libarary));
            }
        }
        else if (select == 1) {
            SoundEffect::EraseSoundEffect();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Option));
        }
        else if (select == 2) {
            SoundEffect::EraseSoundEffect();
            Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Credit));
        }
        else if (select == 3) {
            SoundEffect::EraseSoundEffect();
            Engine::GetGameStateManager().ClearNextGameState();
        }
    }

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        SoundEffect::EraseSoundEffect();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }

    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Mainmenu::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
}

void Mainmenu::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::MainmenuBGM);
        SoundFlag = false;
    }
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
    SoundEffect::EraseSoundEffect();
}