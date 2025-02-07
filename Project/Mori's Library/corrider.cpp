#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "corridor.h"
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
#include "wall.h"

void Corridor::Load()
{
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0,0 }, { 30, 720 }));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    GetGSComponent<Background>()->Add("assets/sprite/Background/Col_Tile.spt", 1, vec2(600, 2400));

    player_ptr = new Player({ 0, -500 }, vec2(50));
    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(0, 1000), vec2(540, 400), "Collider_Window"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(280, 0), vec2(40, 2500), "LibraryWall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-280, 0), vec2(40, 2500), "LibraryWall"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(100, 0), vec2(100), "coffee_table", 0.5f));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-100, 200), vec2(100), "coffee_table", -0.5f));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-20, -300), vec2(62), "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-80, -300), vec2(62), "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(50, -400), vec2(100), "coffee_table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-70, 450), vec2(62), "table", 0.7f));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-120, 450), vec2(62), "table", 0.3f));



    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, 400), static_cast<int>(States::Tutorial), "TutorialBook"));
    announcement1 = new FontObject(vec2(0, 550), "Tutorial");
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, 750), static_cast<int>(States::Libarary), "eddy", Engine::whirl));

    GetGSComponent<GameObjectManager>()->Add(player_ptr);

    AddGSComponent(new ParticleManager<Particles::Smoke>());
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));

    clear_effect = true;
    clear = false;
}

void Corridor::Update(float dt)
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

}

void Corridor::Draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();
}

void Corridor::check_clear([[maybe_unused]] float dt)
{

}

void Corridor::end_effect()
{
    clear_effect = false;

    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, -300), static_cast<int>(States::Mainmenu), "eddy", Engine::hit));
}

void Corridor::Unload()
{
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}