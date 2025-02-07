#include <iostream>

#include "Engine/Engine.h"
#include "Mori/Splash.h"
#include "Mori/Mainmenu.h"
#include "Mori/Book1.h"
#include "Mori/Book2.h"
#include "Mori/Book4.h"
#include "Mori/Book3.h"
#include "Mori/BookN.h"
#include "Mori/Library.h"
#include "Mori/Credit.h"
#include "Mori/Option.h"
#include "Mori/Bible.h"
#include "Mori/Fallen_Bible.h"
#include <GLApp.h>
#include "Tutorial.h"
#include "corridor.h"
#include "Mori/Book5.h"
#include "Mori/Rolling_Credit.h"
#include "Mori/Basement.h"

constexpr static int Width = 1280;
constexpr static int Height = 720;
bool book1_clear[2] = { false, false };
bool book2_clear = false;
bool book3_clear[2] = { false, false };
bool book4_clear[2] = { false, false };
bool book5_clear[2] = { false, false };
bool bible_clear[2] = { false, false };

util::owner<IProgram*> create_program(int viewport_width, int viewport_height)
{
    Engine::Instance()->Set_Window(static_cast<float>(viewport_width), static_cast<float>(viewport_height));
    SoundEffect::InitializeStaticSounds();
    return Engine::Instance();
}

#ifdef _DEBUG
int main() {
#else
int WinMain() {
#endif
    try {
        GLApp OpenGLApplication("Namu's library", Width, Height);

        Engine* engine = Engine::Instance();

        engine->AddFont("assets/images/Font_Outlined.png");
        
        Splash splash;
        engine->GetGameStateManager().AddGameState(splash);
        Mainmenu mainmenu;
        engine->GetGameStateManager().AddGameState(mainmenu);
        Library library;
        engine->GetGameStateManager().AddGameState(library);
        Book1 book1;
        engine->GetGameStateManager().AddGameState(book1);
        Book2 book2;
        engine->GetGameStateManager().AddGameState(book2);
        Book3 book3;
        engine->GetGameStateManager().AddGameState(book3);
        Book4 book4;
        engine->GetGameStateManager().AddGameState(book4);
        Book5 book5;
        engine->GetGameStateManager().AddGameState(book5);
        BookN bookn;
        engine->GetGameStateManager().AddGameState(bookn);
        Bible bible;
        engine->GetGameStateManager().AddGameState(bible);
        Fallen_Bible fallen_bible;
        engine->GetGameStateManager().AddGameState(fallen_bible);
        Credit credit;
        engine->GetGameStateManager().AddGameState(credit);
        Option option;
        engine->GetGameStateManager().AddGameState(option);
        Tutorial tutorial;
        engine->GetGameStateManager().AddGameState(tutorial);
        Corridor corridor;
        engine->GetGameStateManager().AddGameState(corridor);
        Rolling_Credit rolling_credit;
        engine->GetGameStateManager().AddGameState(rolling_credit);
        Basement basement;
        engine->GetGameStateManager().AddGameState(basement);

        while (OpenGLApplication.IsDone() == false && Engine::Instance()->HasGameEnded() == false)
        {
            OpenGLApplication.Update();
        }

        return 0;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
}



