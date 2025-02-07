#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "Tutorial.h"
#include "../Mori/Player.h"
#include "../Mori/SkillUI.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "../Mori/Life.h"
#include "../Mori/Background.h"
#include "../ScreenEffect.h"
#include "../Mori/portalbook.h"
#include "tuto_dummy.h"
#include "Tuto_ball.h"
#include "Mori.h"

extern float SOUND;

void Tutorial::Load()
{
    background_sound = SOUND - 15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }
    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::TutorialBGM);
        SoundEffect::Loop_SoundEffect(SFX::TutorialBGM);
        SoundEffect::SetSoundEffectVolume(SFX::TutorialBGM, background_sound);
        SoundFlag = true;
    }

    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, Engine::Instance()->Get_iWindow() / 8));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    GetGSComponent<Background>()->Add("assets/sprite/Background/Tutorial_Background.spt", 1, Engine::Instance()->Get_Window() * 1.5f);
    
    player_ptr = new Player({ 0, -200 }, vec2(50));
    GetGSComponent<GameObjectManager>()->Add(player_ptr);

    mori = new Mori(vec2(0, 0), vec2(200));
    GetGSComponent<GameObjectManager>()->Add(mori);

    mori_talk = new Mori_talk(*mori, *player_ptr);
    GetGSComponent<GameObjectManager>()->Add(mori_talk);


    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));

    AddGSComponent(new ParticleManager<Particles::Tuto_ball>());



    AddGSComponent(new ParticleManager<Particles::Smoke>());
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));

    clear_effect = true;
    clear = false;
}

void Tutorial::Update(float dt)
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
    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        SoundEffect::EraseSoundEffect();
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }

}

void Tutorial::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();
}

void Tutorial::check_clear([[maybe_unused]] float dt)
{
    if (mori_talk->tuto_animation_end()) {
        clear = true;
    }
}

void Tutorial::end_effect()
{
    GetGSComponent<GameObjectManager>()->Remove(GetGSComponent<GameObjectManager>()->GetObject<tuto_dummy>());
    clear_effect = false;
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(-200, 0), static_cast<int>(States::Libarary), "eddy", Engine::whirl));
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(200, 0), static_cast<int>(States::Libarary), "eddy", Engine::whirl));
}

void Tutorial::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::TutorialBGM);
        SoundFlag = false;
    }
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
    SoundEffect::EraseSoundEffect();
}