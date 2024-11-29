#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "Bible.h"
#include "States.h"
#include "Player.h"
#include "Lucifer.h"
#include "Angel_wings.h"
#include "Blessed_Aura.h"
#include "Light_spear.h"
#include "Javelin.h"
#include "SkillUI.h"
#include "portalbook.h"
#include "Final_Spark.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "Life.h"
#include "../ScreenEffect.h"
#include "portalbook_image.h"
#include "GameOver.h"
#include "Background.h"
#include "../Mori.h"
extern float SOUND;

extern bool bible_clear[2];

void Bible::Load()
{
    background_sound = SOUND - 15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }
    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::BibleBGM);
        SoundEffect::Loop_SoundEffect(SFX::BibleBGM);
        SoundEffect::SetSoundEffectVolume(SFX::BibleBGM, background_sound);
        SoundFlag = true;
    }
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, Engine::Instance()->Get_iWindow() / 2));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);

    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));

    the_end = new portalbook(vec2(-3000, -3000), static_cast<int>(States::Rolling_Credit), "portalbook", Engine::bright);

    player_ptr = new Player({ 0, -200 }, vec2(50));
    lucifer_ptr = new Lucifer(vec2(0, 0), *player_ptr);
    aura_ptr = new Blessed_Aura(vec2(0, 0), *player_ptr);
    angel_wings_ptr = new Angel_wings(vec2(0, 0), *lucifer_ptr);
    gameover = new GameOver(vec2(2000, 2000));
    gameclear = new GameClear(vec2(2000, 2000));
    pressanykey = new PressAnyKey(vec2(2000, 2000));
    darkscreen = new Screen_Effect(vec2(5000, 5000));

    GetGSComponent<Background>()->Add("assets/sprite/Background/Bible_back.spt", 1, Engine::Instance()->Get_Window() * 2);

    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));
    AddGSComponent(new ParticleManager<Particles::FinalSpark>());

    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    GetGSComponent<GameObjectManager>()->Add(aura_ptr);
    AddGSComponent(new ParticleManager<Particles::Smoke>());
    AddGSComponent(new ParticleManager<Particles::Divine_light>());
    AddGSComponent(new ParticleManager<Particles::Light_spear>());
    AddGSComponent(new ParticleManager<Particles::Javelin>());
    AddGSComponent(new ParticleManager<Particles::Explode>());

    GetGSComponent<ParticleManager<Particles::FinalSpark>>()->SetPtr(lucifer_ptr);
    GetGSComponent<ParticleManager<Particles::FinalSpark>>()->Inputindex();
    GetGSComponent<ParticleManager<Particles::Light_spear>>()->SetPtr(lucifer_ptr);
    GetGSComponent<ParticleManager<Particles::Light_spear>>()->Inputindex();
    GetGSComponent<ParticleManager<Particles::Javelin>>()->SetPtr(lucifer_ptr);
    GetGSComponent<ParticleManager<Particles::Javelin>>()->Inputindex();

    GetGSComponent<GameObjectManager>()->Add(angel_wings_ptr);
    GetGSComponent<GameObjectManager>()->Add(lucifer_ptr);
    GetGSComponent<GameObjectManager>()->Add(the_end);
    dead_cooltime = 0;
    Sound_One_Time = false;
    stop_move_one_time = false;
    GetGSComponent<GameObjectManager>()->Add(darkscreen);
    GetGSComponent<GameObjectManager>()->Add(gameclear);
    GetGSComponent<GameObjectManager>()->Add(gameover);
    GetGSComponent<GameObjectManager>()->Add(pressanykey);
}

void Bible::Update(float dt)
{
    if (clear == false) {
        check_clear(dt);
    }
    if (clear && clear_effect) {
        end_effect();
    }
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());
    GetGSComponent<UIManager>()->Update(dt);
    if (player_ptr->GetGOComponent<Life>()->Dead() == true) {
        if (Sound_One_Time == false) {
            SoundEffect::Stop_Loop_SoundEffect(SFX::BibleBGM);
            SoundEffect::EraseSoundEffect();
            SoundEffect::PlaySoundEffect(SFX::Death);
            SoundEffect::Loop_SoundEffect(SFX::Death);
            SoundEffect::SetSoundEffectVolume(SFX::Death, background_sound);
            Sound_One_Time = true;
        }
        gameover->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
        pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
        GetGSComponent<GameObjectManager>()->Remove(player_ptr);
        darkscreen->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y));

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
        SoundEffect::EraseSoundEffect();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }

}

void Bible::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();

}

void Bible::check_clear([[maybe_unused]] float dt)
{
    if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
        if (GetGSComponent<GameObjectManager>()->GetObject<Lucifer>() == nullptr)
        {
            if (Sound_One_Time == false) {
                SoundEffect::Stop_Loop_SoundEffect(SFX::BibleBGM);
                SoundEffect::EraseSoundEffect();
                SoundEffect::PlaySoundEffect(SFX::ClearBGM);
                SoundEffect::SetSoundEffectVolume(SFX::ClearBGM, background_sound);
                // Add mugic

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
    else if(Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        if (lucifer_ptr->LuciferDead() == true) {
            clear = true;
        }
    }
}

void Bible::end_effect()
{
    clear_effect = false;
    if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
        player_ptr->SetPosition(vec2(0, 0));
        GetGSComponent<GameObjectManager>()->Add(new Mori(vec2(200, 0), vec2(200)));
        GetGSComponent<GameObjectManager>()->Add(new Mori_talk(*GetGSComponent<GameObjectManager>()->GetObject<Mori>(), *player_ptr));
        GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, -300), static_cast<int>(States::Fallen_Bible), "Hole", Engine::Dive_hole));
        SoundEffect::Stop_Loop_SoundEffect(SFX::BibleBGM);
    }
    else {
        bible_clear[0] = true;
        the_end->SetPosition(vec2(0, 0));
        the_end->SetScale(Engine::Instance()->Get_Window() * 4);
    }
}

void Bible::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::BibleBGM);
        SoundEffect::StopSoundEffect(SFX::Death);
        SoundEffect::Stop_Loop_SoundEffect(ClearBGM);
        SoundEffect::StopSoundEffect(SFX::ClearBGM);
        SoundFlag = false;
    }
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}