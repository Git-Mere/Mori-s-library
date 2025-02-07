#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "Fallen_Bible.h"
#include "States.h"
#include "Player.h"
#include "Fallen_Lucifer.h"
#include "Fallen_Angel_wings.h"
#include "SkillUI.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "Life.h"
#include "Blessed_Aura.h"
#include "Fallen_Attack.h"
#include "Javelin.h"
#include "Light_spear.h"
#include "Final_Spark.h"
#include "../Mori/portalbook.h"
#include "../ScreenEffect.h"
#include "GameOver.h"
#include "Background.h"

extern float SOUND;
extern bool bible_clear[2];

void Fallen_Bible::Load()
{
    background_sound = SOUND - 15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }
    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::FallenBibleBGM);
        SoundEffect::Loop_SoundEffect(SFX::FallenBibleBGM);
        SoundEffect::SetSoundEffectVolume(SFX::FallenBibleBGM, background_sound);
        SoundFlag = true;
    }
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, Engine::Instance()->Get_iWindow() / 2));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));

    the_end = new portalbook(vec2(-3000, -3000), static_cast<int>(States::Rolling_Credit), "portalbook", Engine::bright);

    GetGSComponent<Background>()->Add("assets/sprite/Background/Fallen_Bible_back.spt", 1, Engine::Instance()->Get_Window() * 2);

    player_ptr = new Player({ 0, -200 }, vec2(50));
    aura_ptr = new Blessed_Aura(vec2(0, 0), *player_ptr);
    fallen_lucifer_ptr = new Fallen_Lucifer(vec2(0, 0), *player_ptr);
    fallen_angel_wings_ptr = new Fallen_Angel_wings(vec2(0, 0), *fallen_lucifer_ptr);
    gameover = new GameOver(vec2(2000, 2000));
    gameclear = new GameClear(vec2(2000, 2000));
    pressanykey = new PressAnyKey(vec2(2000, 2000));
    darkscreen = new Screen_Effect(vec2(5000, 5000));

    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));
    stop_move_one_time = false;

    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    GetGSComponent<GameObjectManager>()->Add(aura_ptr);
    AddGSComponent(new ParticleManager<Particles::DarkSpark>());
    AddGSComponent(new ParticleManager<Particles::Lazer>());
    GetGSComponent<GameObjectManager>()->Add(fallen_angel_wings_ptr);
    GetGSComponent<GameObjectManager>()->Add(fallen_lucifer_ptr);

    AddGSComponent(new ParticleManager<Particles::Divine_Dark>());
    AddGSComponent(new ParticleManager<Particles::Divine_Dark2>());
    AddGSComponent(new ParticleManager<Particles::Fire_Pillar>());
    AddGSComponent(new ParticleManager<Particles::Fire_Pit>());
    AddGSComponent(new ParticleManager<Particles::Dark_Javelin>());
    AddGSComponent(new ParticleManager<Particles::Dark_Spear>());
    AddGSComponent(new ParticleManager<Particles::Smoke>());

    AddGSComponent(new ParticleManager<Particles::Explode>());
    GetGSComponent<ParticleManager<Particles::Dark_Javelin>>()->SetPtr(fallen_lucifer_ptr);
    GetGSComponent<ParticleManager<Particles::Dark_Javelin>>()->Inputindex();
    GetGSComponent<ParticleManager<Particles::Dark_Spear>>()->SetPtr(fallen_lucifer_ptr);
    GetGSComponent<ParticleManager<Particles::Dark_Spear>>()->Inputindex();
    GetGSComponent<ParticleManager<Particles::DarkSpark>>()->SetPtr(fallen_lucifer_ptr);
    GetGSComponent<ParticleManager<Particles::DarkSpark>>()->Inputindex();
    GetGSComponent<GameObjectManager>()->Add(the_end);
    dead_cooltime = 0;
    Sound_One_Time = false;
    stop_move_one_time = false;
    GetGSComponent<GameObjectManager>()->Add(darkscreen);
    GetGSComponent<GameObjectManager>()->Add(gameover);
    GetGSComponent<GameObjectManager>()->Add(gameclear);
    GetGSComponent<GameObjectManager>()->Add(pressanykey);
}

void Fallen_Bible::Update(float dt)
{
    if (clear == false) {
        check_clear(dt);
    }
    if (clear && clear_effect) {
        end_effect();
    }
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->Update(dt);
    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());


    if (player_ptr->GetGOComponent<Life>()->Dead() == true) {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::FallenBibleBGM);
            SoundEffect::EraseSoundEffect();
            // Add mugic
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
                Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Basement));
            }
        }
    }

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }

}

void Fallen_Bible::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();
}

void Fallen_Bible::check_clear([[maybe_unused]] float dt)
{
    if (GetGSComponent<GameObjectManager>()->GetObject<Fallen_Lucifer>() == nullptr)
    {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::FallenBibleBGM);
            SoundEffect::StopSoundEffect(SFX::FallenBibleBGM);
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

void Fallen_Bible::end_effect()
{
    bible_clear[1] = true;
    the_end->SetPosition(vec2(0, 0));
    the_end->SetScale(Engine::Instance()->Get_Window() * 4);
}

void Fallen_Bible::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::FallenBibleBGM);
        SoundEffect::StopSoundEffect(SFX::FallenBibleBGM);
        SoundEffect::StopSoundEffect(SFX::Death);
        SoundFlag = false;
    }
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}