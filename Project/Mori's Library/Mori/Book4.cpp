#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "Book4.h"
#include "States.h"
#include "Slime.h"
#include "Player.h"
#include "SkillUI.h"
#include "Life.h"
#include "Fiter.h"
#include "Background.h"
#include "../Mori/portalbook.h"
#include "FiterAttack.h"
#include "../ScreenEffect.h"
#include "../wall.h"
#include "GameOver.h"

extern float SOUND;
extern bool book4_clear[2];

void Book4::Load()
{
    background_sound = SOUND - 15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }
    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::Book4BGM);
        SoundEffect::Loop_SoundEffect(SFX::Book4BGM);
        SoundEffect::SetSoundEffectVolume(SFX::Book4BGM, background_sound);
        SoundFlag = true;
    }
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, Engine::Instance()->Get_iWindow() / 4));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);

    glCheck(glClearColor(0.0f, 0.68f, 0.94f, 1.0f));

    gameover = new GameOver(vec2(2000, 2000));
    gameclear = new GameClear(vec2(2000, 2000));
    pressanykey = new PressAnyKey(vec2(2000, 2000));
    darkscreen = new Screen_Effect(vec2(5000, 5000));

    player_ptr = new Player(vec2( 0, -200 ), vec2(50));
    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));

    blossoming_blade_ptr = new Fiter(vec2(0,0), *player_ptr);
    GetGSComponent<Background>()->Add("assets/sprite/Book4/Book4_background.spt", 1, Engine::Instance()->Get_Window() * 2);
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));

    AddGSComponent(new ParticleManager<Particles::PunchEarthquake>());
    AddGSComponent(new ParticleManager<Particles::Energy_Lazer_L>());
    AddGSComponent(new ParticleManager<Particles::Energy_Lazer_R>());
    AddGSComponent(new ParticleManager<Particles::B_Energy_Lazer_L>());
    AddGSComponent(new ParticleManager<Particles::B_Energy_Lazer_R>());
    //GetGSComponent<GameObjectManager>()->Add(blossoming_blade_ptr);
    AddGSComponent(new ParticleManager<Particles::BossHit>());

    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    AddGSComponent(new ParticleManager<Particles::Smoke>());

    AddGSComponent(new ParticleManager<Particles::Energy_Kick1>());
    AddGSComponent(new ParticleManager<Particles::Energy_Kick2>());
    AddGSComponent(new ParticleManager<Particles::Energy_Fist>());
    if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        vec2 side = vec2(900, 0);
        vec2 updown = vec2(0, 700);
        
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-650, -350), vec2(300, 1000), "None", 0.8f));
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(650, -350), vec2(300, 1000), "None", 2.4f));
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(650, 350), vec2(300, 1000), "None", 0.8f));
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-650, 350), vec2(300, 1000), "None", 2.4f));

        GetGSComponent<GameObjectManager>()->Add(new Wall(updown, vec2(3000, 400), "None"));
        GetGSComponent<GameObjectManager>()->Add(new Wall(-updown, vec2(3000, 400), "None"));
        GetGSComponent<GameObjectManager>()->Add(new Wall(side, vec2(600, 3000), "None"));
        GetGSComponent<GameObjectManager>()->Add(new Wall(-side, vec2(600, 3000), "None"));

    }

    stop_move_one_time = false;
    clear_effect = true;
    clear = false;
    dead_cooltime = 0;
    Sound_One_Time = false;
    GetGSComponent<GameObjectManager>()->Add(darkscreen);
    GetGSComponent<GameObjectManager>()->Add(gameclear);
    GetGSComponent<GameObjectManager>()->Add(gameover);
    GetGSComponent<GameObjectManager>()->Add(pressanykey);
}

void Book4::Update(float dt)
{
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->Update(dt);
    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());

    if (clear == false) {
        check_clear(dt);
    }
    if (clear && clear_effect) {
        end_effect();
    }

    if (player_ptr->GetGOComponent<Life>()->Dead() == true) {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::Book4BGM);
            SoundEffect::EraseSoundEffect();
            SoundEffect::PlaySoundEffect(SFX::Death);
            SoundEffect::Loop_SoundEffect(SFX::Death);
            SoundEffect::SetSoundEffectVolume(SFX::Death, background_sound);
            Sound_One_Time = true;
        }
        gameover->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
        pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
        GetGSComponent<GameObjectManager>()->Remove(player_ptr);
        darkscreen->RePosition( vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y) );

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

void Book4::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();
}

void Book4::check_clear([[maybe_unused]] float dt)
{
    if (GetGSComponent<GameObjectManager>()->GetObject<Fiter>() == nullptr)
    {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::Book4BGM);
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

void Book4::end_effect()
{
    if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
        book4_clear[0] = true;
    }
    else if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        book4_clear[1] = true;
    }
    clear_effect = false;
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, -300), static_cast<int>(States::Libarary), "eddy", Engine::whirl));
}

void Book4::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::Book4BGM);
        SoundEffect::StopSoundEffect(SFX::Death);
        SoundEffect::StopSoundEffect(SFX::ClearBGM);
        SoundFlag = false;
    }
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}