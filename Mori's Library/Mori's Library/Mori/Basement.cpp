#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "Basement.h"
#include "Player.h"
#include "portalbook.h"
#include "portalbook_image.h"
#include "States.h"
#include "SkillUI.h"
#include "FontObject.h"
#include "../Engine/Particels.h"
#include "Background.h"
#include "../wall.h"
#include "../ScreenEffect.h"
#include "../Furniture.h"
#include "../Mori.h"


void Basement::Load()
{
    glCheck(glClearColor(0.f, 0.0f, 0.f, 1.0f));

    if (Engine::GetGameStateManager().Get_last_state_name() == "Libarary" ) {
        player_last_position = vec2(0, -350);
    }
    else {
        if (player_last_velocity.x > 0) {
            player_last_position -= vec2(70, 0);
        }
        else if (player_last_velocity.x <= 0) {
            player_last_position += vec2(70, 0);
        }
    }

    player_ptr = new Player(player_last_position, vec2(50));

    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent(player_last_position, {30, 200}));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));
    AddGSComponent(new ParticleManager<Particles::Smoke>());

    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));

    GetGSComponent<Background>()->Add("assets/sprite/Background/Basement_Background.spt", 1, vec2(800, 1200));



    initial_Wall();

    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    AddGSComponent(new ParticleManager<Particles::Smoke>());
}

void Basement::Update(float dt)
{
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());
    GetGSComponent<UIManager>()->Update(dt);

    player_last_position = player_ptr->GetPosition();
    if (player_ptr->GetVelocity().x != 0) {
        player_last_velocity = player_ptr->GetVelocity();
    }
}

void Basement::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());

    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();

}

void Basement::initial_Wall()
{
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(200, -400), vec2(40, 500), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-200, -400), vec2(40, 500), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(410, 200), vec2(40, 700), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-410, 200), vec2(40, 700), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(305, -150), vec2(250, 40), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-305, -150), vec2(250, 40), "LibraryWall1"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-280, 600), vec2(300, 210), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(20, 600), vec2(300, 210), "LibraryWall1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(280, 600), vec2(300, 210), "LibraryWall1"));

    GetGSComponent<GameObjectManager>()->Add(new Furniture(vec2(0, 170), Engine::Instance()->Get_Window() / 2, "carpet"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-230, 500), vec2(200), "base_Bookshelf1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-330, 500), vec2(200), "base_Bookshelf1"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-350, -90), vec2(70), "table1"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-270, -90), vec2(70), "table1"));
    
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, -500), static_cast<int>(States::Libarary), "Exit2", Engine::Basement));

    GetGSComponent<GameObjectManager>()->Add(new Mori(vec2(200, 100), vec2(200)));
    GetGSComponent<GameObjectManager>()->Add(new Mori_talk(*GetGSComponent<GameObjectManager>()->GetObject<Mori>(), *player_ptr));
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, 200), static_cast<int>(States::Bible), "BibleBook"));

}

void Basement::Unload()
{
    SoundEffect::EraseSoundEffect();
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}