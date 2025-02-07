#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "Library.h"
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
#include "book_scroll.h"

extern float SOUND;

extern bool book1_clear[2];
extern bool book2_clear;
extern bool book3_clear[2];
extern bool book4_clear[2];
extern bool book5_clear[2];
extern bool bible_clear[2];

void Library::Load()
{   
    background_sound = SOUND-15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }
    //Engine::Instance()->Set_Gamemode(Engine::Hard);
    SoundEffect::SetSoundEffectVolume(SFX::LibraryBGM, background_sound);
    SoundEffect::PlaySoundEffect(SFX::LibraryBGM);
    SoundEffect::Loop_SoundEffect(SFX::LibraryBGM);

    if (Engine::GetGameStateManager().Get_last_state_name() == "Corridor" || Engine::GetGameStateManager().Get_last_state_name() == "Mainmenu" ||
        Engine::GetGameStateManager().Get_last_state_name() == "Splash" || Engine::GetGameStateManager().Get_last_state_name() == "basement") {
        player_last_position = vec2(0,0);
    }
    else {
        if (player_last_velocity.x > 0 ) {
            player_last_position -= vec2(70, 0);
        }
        else if (player_last_velocity.x <= 0) {
            player_last_position += vec2(70, 0);
        }
    }

    SoundEffect::StopSoundEffect(SFX::Player_walk);
    glCheck(glClearColor(1.f, 0.68f, 1.f, 1.0f));
    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent(player_last_position, Engine::Instance()->Get_iWindow() / 2));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));

    player_ptr = new Player(player_last_position, vec2(50));

    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));

    GetGSComponent<Background>()->Add("assets/sprite/Background/Tile.spt", 1, Engine::Instance()->Get_Window() * 2);

    //funiture
    GetGSComponent<GameObjectManager>()->Add(new Furniture(vec2(0, 0), Engine::Instance()->Get_Window() / 2, "carpet"));

    //object
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(0, -600), static_cast<int>(States::Mainmenu), "eddy", Engine::whirl));
    Basement_potal = new portalbook(vec2(0, 1000), static_cast<int>(States::Basement), "Exit", Engine::Basement);


    initial_Wall();

    sit_mori = new Mori(vec2(0, 350), vec2(350, 300));
    GetGSComponent<GameObjectManager>()->Add(sit_mori);
    GetGSComponent<GameObjectManager>()->Add(new Mori_talk(*GetGSComponent<GameObjectManager>()->GetObject<Mori>(), *player_ptr));

   //GetGSComponent<GameObjectManager>()->Add(basement);
    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    GetGSComponent<GameObjectManager>()->Add(Basement_potal);

    //effect
    AddGSComponent(new ParticleManager<Particles::Smoke>());
}

void Library::Update(float dt)
{
    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());
    GetGSComponent<UIManager>()->Update(dt);
    player_last_position = player_ptr->GetPosition();
    if(player_ptr->GetVelocity().x != 0){
        player_last_velocity = player_ptr->GetVelocity();
    }
#ifdef _DEBUG
    if (Engine::GetInput().KeyJustReleased(Input::Keys::R)) {
        std::cout << Engine::Instance()->Get_Game_mode() << std::endl;
        Engine::Instance()->Set_Gamemode(
            Engine::Instance()->Get_Game_mode() == Engine::Easy ?
            Engine::Hard : Engine::Easy
        );
    }
#endif
    if (Basement_potal->GetPosition() == vec2(0, 1000)) {
        if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
            if (book1_clear[0] && book2_clear && book3_clear[0] && book4_clear[0] && book5_clear[0]) {
                Basement_potal->ChangePosition(vec2(0, 100));
                sit_mori->GetGOComponent<Sprite>()->PlayAnimation(1);
                sit_mori->no_mori = true;
            }
        }
        else if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
            if (book1_clear[1] && book2_clear && book3_clear[1] && book4_clear[1] && book5_clear[1]) {
                Basement_potal->ChangePosition(vec2(0, 100));
                sit_mori->GetGOComponent<Sprite>()->PlayAnimation(1);
                sit_mori->no_mori = true;
            }
        }
    }
    else {
        if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
            if (!book1_clear[0] || !book2_clear || !book3_clear[0] || !book4_clear[0] || !book5_clear[0]) {
                Basement_potal->ChangePosition(vec2(0, 1000));
                sit_mori->GetGOComponent<Sprite>()->PlayAnimation(0);
                sit_mori->no_mori = false;
            }
        }
        else if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
            if (!book1_clear[1] || !book2_clear || !book3_clear[1] || !book4_clear[1] || !book5_clear[1]) {
                Basement_potal->ChangePosition(vec2(0, 1000));
                sit_mori->GetGOComponent<Sprite>()->PlayAnimation(0);
                sit_mori->no_mori = false;
            }
        }
    }
}

void Library::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());

    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();

}
//
void Library::initial_Wall()
{
    vec2 standard = vec2(100, 100);

    //up
    for (int i = 0; i <= 7; i++) {
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1350 + (280.0f * (2 * i)), 620), vec2(280, 210), "Window"));
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1350 + (280.0f * (2 * i + 1)), 620), vec2(280, 210), "LibraryWall"));
    }

    //1
    vec2 one_col = vec2(600, 200);
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_col + vec2(0, 280)    , standard * 2 + vec2(60, 0), "Long_Wall2"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_col + vec2(-90, 160)  , standard * 3, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_col + vec2(90, 160)   , standard * 3, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_col + vec2(0, 190)    , standard + vec2(25, 25), "None"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_col, standard * 3 + vec2(100, 0), "Bookshelf"));

    vec2 one_two_col = vec2(1200, 0);
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_two_col + vec2(0, 180), standard + vec2(20, 0), "Long_Wall2"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_two_col + vec2(-40, 100), standard * 2, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_two_col + vec2(40, 100), standard * 2, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_two_col + vec2(0, 105), standard + vec2(-50, 20), "None"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(one_two_col, standard * 2, "Bookshelf"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(300, 600), standard * 2 + vec2(100, 0), "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(300, 500), standard * 2 + vec2(100, 0), "Bookshelf"));


    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(900, 300), standard * 0.75f, "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(1100, 300), standard * 0.75f, "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(1000, 200), standard * 0.75f, "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(1000, 400), standard * 0.75f, "table"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(1000, 320), standard * 2.f, "coffee_table"));

    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(1000, -200), static_cast<int>(States::Book3), "PeterPan"));


    //2
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-400, 600), standard * 3, "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-570, 600), standard * 3, "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1000, 600), standard * 3, "Bookshelf"));


    vec2 two_col = vec2(-700, 300);
    GetGSComponent<GameObjectManager>()->Add(new Wall(two_col+ vec2(0, 180)  , standard + vec2(20, 0), "Long_Wall2"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(two_col+ vec2(-40, 100), standard * 2, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(two_col+ vec2(40, 100) , standard * 2, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(two_col+ vec2(0, 105), standard + vec2(-50, 20), "None"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(two_col, standard * 2, "Bookshelf"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1000, 100), standard * 2 + vec2(0,200), "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1100, 100), standard * 2 + vec2(0, 200), "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1200, 100), standard * 2 + vec2(0, 200), "Long_Wall"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-300, 350), standard * 0.75f, "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-500, 350), standard * 0.75f, "table"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-400, 400), standard * 2.f, "coffee_table"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-420, 0), standard * 0.75f, "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-420, -100), standard * 0.75f, "table"));

    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2( -1050, 400), static_cast<int>(States::Book4), "BlossomBook"));


    //3
    vec2 three_col = vec2(-600, -200);
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(0, 280), standard * 2 + vec2(60, 0), "Long_Wall2"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(-90, 160), standard * 3, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(90, 160), standard * 3, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col, standard * 3 + vec2(100, 0), "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(0, 190), standard + vec2(25, 25), "None"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(-600, -400), standard * 3, "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(-600, -500), standard * 3, "Bookshelf"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(-300, -400), standard * 3, "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(three_col + vec2(-300, -500), standard * 3, "Bookshelf"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1200, -150), standard * 1.5f, "coffee_table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1200, -240), standard, "table"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-1200, -310), standard, "table"));

    for (int i = 1; i <= 7; i++) {
        float add = 100.f;
        GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(-870 + add * i, -670), standard * 0.75f, "table"));
    }
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(-1000, -400), static_cast<int>(States::Book2), "QuizBook"));
    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(-420, -200), static_cast<int>(States::Book1), "SlimeBook"));


    //4
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(700, -400), standard * 2+ vec2(30,100), "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(900, -400), standard * 2 + vec2(30, 100), "Bookshelf"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(1100, -400), standard * 2+ vec2(30, 100), "Bookshelf"));

    vec2 four_col = vec2(400, -300);
    GetGSComponent<GameObjectManager>()->Add(new Wall(four_col + vec2(0, 180), standard + vec2(20, 0), "Long_Wall2"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(four_col + vec2(-40, 100), standard * 2, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(four_col + vec2(40, 100), standard * 2, "Long_Wall"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(four_col + vec2(0, 105), standard + vec2(-50, 20), "None"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(four_col, standard * 2, "Bookshelf"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(440, -620), standard * 0.75f, "table"));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(400, -700), standard + vec2(400,0), "Bookshelf"));

    GetGSComponent<GameObjectManager>()->Add(new portalbook(vec2(1000, -600), static_cast<int>(States::Book5), "Book5"));

    //ModeBook
    GetGSComponent<GameObjectManager>()->Add(new portalbook_image(vec2(300, 350), "ModeBook"));

    //Mori
    //mori = new Mori(vec2(0, 350), vec2(350, 300));
    //GetGSComponent<GameObjectManager>()->Add(mori);
    //GetGSComponent<GameObjectManager>()->Add(new Mori_talk(*mori));

}

void Library::Unload()
{
    SoundEffect::EraseSoundEffect();
    GetGSComponent<GameObjectManager>()->Unload();
    ClearGSComponents();
}