#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "Book5.h"
#include "States.h"
#include "Bard.h"
#include "Player.h"
#include "SkillUI.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "Life.h"
#include "Background.h"
#include "Cosmic_tone.h"
#include <iostream>
#include "Parrying_attack.h"
#include "../Mori/portalbook.h"
#include "../ScreenEffect.h"
#include "GameOver.h"

extern bool book5_clear[2];

extern float SOUND;
void Book5::Load()
{
    background_sound = SOUND - 15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }
    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::Book5BGM);
        SoundEffect::Loop_SoundEffect(SFX::Book5BGM);
        SoundEffect::SetSoundEffectVolume(SFX::Book5BGM, background_sound);
        SoundFlag = true;
    }
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, Engine::Instance()->Get_iWindow() / 2));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    GetGSComponent<Background>()->Add("assets/sprite/Book5/Book5_background.spt", 1, Engine::Instance()->Get_Window() * 2);
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));
    glCheck(glClearColor(0.0f, 0.68f, 0.94f, 1.0f));


    player_ptr = new Player({ 0, -200 }, vec2(50));
    bard_ptr = new Bard(vec2(0, 0), *player_ptr);
    gameclear = new GameClear(vec2(2000, 2000));
    gameover = new GameOver(vec2(2000, 2000));
    pressanykey = new PressAnyKey(vec2(2000, 2000));
    darkscreen = new Screen_Effect(vec2(5000, 5000));

    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));

    GetGSComponent<GameObjectManager>()->Add(bard_ptr);
    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    AddGSComponent(new ParticleManager<Particles::Smoke>());
    AddGSComponent(new ParticleManager<Particles::Cosmic_tone>());
    AddGSComponent(new ParticleManager<Particles::Slime_ball>());

    AddGSComponent(new ParticleManager<Particles::Parrying_attack>());

    AddGSComponent(new ParticleManager<Particles::Explode>());
    AddGSComponent(new ParticleManager<Particles::Boom>());
    AddGSComponent(new ParticleManager<Particles::Electronic>());
    AddGSComponent(new ParticleManager<Particles::Electronic_line>());

    GetGSComponent<ParticleManager<Particles::Cosmic_tone>>()->SetPtr(bard_ptr);
    GetGSComponent<ParticleManager<Particles::Cosmic_tone>>()->Inputindex();

    GetGSComponent<ParticleManager<Particles::Parrying_attack>>()->SetPtr(player_ptr);
    GetGSComponent<ParticleManager<Particles::Parrying_attack>>()->Inputindex();

    clear_effect = true;
    clear = false;
    dead_cooltime = 0;
    stop_move_one_time = false;
    Sound_One_Time = false;
    GetGSComponent<GameObjectManager>()->Add(darkscreen);
    GetGSComponent<GameObjectManager>()->Add(gameover);
    GetGSComponent<GameObjectManager>()->Add(gameclear);
    GetGSComponent<GameObjectManager>()->Add(pressanykey);
}

void Book5::Update(float dt)
{
    if (clear == false) {
        check_clear(dt);
    }
    if (clear && clear_effect) {
        end_effect();
    }

    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());

    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->Update(dt);

    if (player_ptr->GetGOComponent<Life>()->Dead() == true) {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::Book5BGM);
            SoundEffect::EraseSoundEffect();
            SoundEffect::PlaySoundEffect(SFX::Death);
            SoundEffect::Loop_SoundEffect(SFX::Death);
            SoundEffect::SetSoundEffectVolume(SFX::Death, background_sound);
            Sound_One_Time = true;
        }
        gameover->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
        pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
        darkscreen->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y));
        GetGSComponent<GameObjectManager>()->Remove(player_ptr);

        if (dead_cooltime < dead_cooltime_max) {
            dead_cooltime += dt;
        }
        else {
            if (Engine::GetInput().PressAnyKey()) {
                Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Libarary));
            }
        }
    }

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        SoundEffect::EraseSoundEffect();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }


}

void Book5::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();
}

void Book5::check_clear([[maybe_unused]] float dt)
{
    if (GetGSComponent<GameObjectManager>()->GetObject<Bard>() == nullptr)
    {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::Book5BGM);
            SoundEffect::EraseSoundEffect();
            SoundEffect::PlaySoundEffect(SFX::ClearBGM);
            SoundEffect::SetSoundEffectVolume(SFX::ClearBGM, background_sound);

            Sound_One_Time = true;
        }

        gameclear->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
        pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
        darkscreen->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y));
        player_ptr->Set_stop_move(true);
        player_ptr->SetVelocity(vec2(0, 0));
        player_ptr->Set_invin_timer(5.0f);

        if (dead_cooltime < dead_cooltime_max) {
            dead_cooltime += dt;
        }
        else {
            if (stop_move_one_time == false) {
                player_ptr->Set_stop_move(false);
                stop_move_one_time = true;
            }
            if (Engine::GetInput().PressAnyKey()) {
                gameclear->RePosition(vec2(2000, 2000));
                pressanykey->RePosition(vec2(2000, 2000));
                darkscreen->RePosition(vec2(5000, 5000));
                clear = true;
            }
        }
    }
}

void Book5::end_effect()
{
    if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
        book5_clear[0] = true;
    }
    else if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        book5_clear[1] = true;
    }
    clear_effect = false;
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, -300), static_cast<int>(States::Libarary), "eddy", Engine::whirl));
}

void Book5::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::Book5BGM);
        SoundEffect::StopSoundEffect(SFX::Death);
        SoundEffect::StopSoundEffect(SFX::ClearBGM);
        SoundFlag = false;
    }
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}