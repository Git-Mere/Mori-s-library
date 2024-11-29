#include "portalbook_image.h"
#include "States.h"
#include "../Engine/GameObjectManager.h"
#include "../ScreenEffect.h"


extern float SOUND;
portalbook_image::portalbook_image(vec2 position, std::string book_name) : GameObject(position), is_book_clear{ false, false }, BookName(book_name)
{
    AddGOComponent(new Sprite("assets/sprite/Portalbook/" + book_name + ".spt", this));
    SetScale(vec2(150));
    SetCollisionScale(vec2(230), true);
    current_state = &state_idle;
    current_state->Enter(this);
    is_game_book = false;
    is_mode_book = false;

    if (BookName == "eddy" || BookName == "Hole" || BookName == "Exit2" || BookName == "Exit") {
        is_sound_need = false;
    }
    if (BookName == "Exit2" || BookName == "Exit") {
        SetScale(vec2(200));

    }

    if (BookName == "ModeBook") {
        is_mode_book = true;
        last_ani = Engine::Instance()->Get_Game_mode() == Engine::Easy ? Animations::Open : Animations::HardOpen;
    }

    if (BookName == "TutorialBook") {
        is_mode_book = true;
        scroll_ptr = new Scroll(vec2(position.x, position.y + 100), BookName, *this);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(scroll_ptr);
    }

    if (BookName == "SlimeBook" || BookName == "QuizBook" || BookName == "PeterPan" || BookName == "BlossomBook" || BookName == "Book5" || BookName == "BibleBook") {
        is_game_book = true;
        scroll_ptr = new Scroll(vec2(position.x, position.y + 100), BookName, *this);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(scroll_ptr);
        Check_Clear(BookName);
        if (now_book_clear) {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Normal_Idle));
        }
        else {
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Gold_Idle));
        }
    }
    else {
        now_book_clear = &is_book_clear[0];
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
    }
}

void portalbook_image::Update(float dt)
{
    GameObject::Update(dt);
    Check_Clear(BookName);
    is_open = false;
}

void portalbook_image::Draw(const mat3& world_to_ndc)
{
    GameObject::Draw(world_to_ndc);
}

bool portalbook_image::CanCollideWith(GameObjectTypes other_object_type)
{
    if (other_object_type == GameObjectTypes::Player) {
        return true;
    }
    return false;
}

void portalbook_image::ResolveCollision(GameObject* other_object)
{
    if (other_object->Type() == GameObjectTypes::Player) {
        is_open = true;
        if (is_mode_book) {
            if (Engine::GetInput().KeyJustReleased(Input::Keys::G)) {
                Engine::GameMode value = Engine::Instance()->Get_Game_mode() == Engine::Easy ? Engine::Hard : Engine::Easy;
                last_ani = value == Engine::Easy ? Animations::Open : Animations::HardOpen;
                int index = static_cast<int>(last_ani);
                Engine::Instance()->Set_Gamemode(value);
                GetGOComponent<Sprite>()->PlayAnimation(index);
            }
        }
    }
    else {
        is_open = false;
    }
}

void portalbook_image::ChangePosition(vec2 position_) {
    SetPosition(position_);
}

void portalbook_image::Check_Clear(std::string book_name)
{
    switch (Engine::Instance()->Get_Game_mode())
    {
    case Engine::Easy:
        if (book_name == "SlimeBook") {
            now_book_clear = &book1_clear[0];
        }
        else if (book_name == "PeterPan") {
            now_book_clear = &book3_clear[0];
        }
        else if (book_name == "BlossomBook") {
            now_book_clear = &book4_clear[0];
        }
        else if (book_name == "Book5") {
            now_book_clear = &book5_clear[0];
        }
        else if (book_name == "BibleBook") {
            now_book_clear = &bible_clear[0];
        }
        break;

    case Engine::Hard:
        if (book_name == "SlimeBook") {
            now_book_clear = &book1_clear[1];
        }
        else if (book_name == "PeterPan") {
            now_book_clear = &book3_clear[1];
        }
        else if (book_name == "BlossomBook") {
            now_book_clear = &book4_clear[1];
        }
        else if (book_name == "Book5") {
            now_book_clear = &book5_clear[1];
        }
        else if (book_name == "BibleBook") {
            now_book_clear = &bible_clear[1];
        }
        break;
    }

    if (book_name == "QuizBook") {
        now_book_clear = &book2_clear;
    }
}

/////////////  animation  /////////////
void portalbook_image::State_Idle::Enter([[maybe_unused]] GameObject* object) {
}

void portalbook_image::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
    portalbook_image* image = static_cast<portalbook_image*>(object);
    if (image->is_game_book == true) {
        if (image->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Normal_Close) || image->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Gold_Close)) {
            if (image->GetGOComponent<Sprite>()->AnimationEnded() == true) {
                if (*image->now_book_clear == false) {
                    image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Normal_Idle));
                }
                else {
                    image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Gold_Idle));
                }
            }
        }
        else {
            if (*image->now_book_clear == false) {
                image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Normal_Idle));
            }
            else {
                image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Gold_Idle));
            }
        }
    }
}

void portalbook_image::State_Idle::CheckExit([[maybe_unused]] GameObject* object) {
    portalbook_image* image = static_cast<portalbook_image*>(object);
    if (image->is_open == true) {
        image->change_state(&image->state_open);
        if (image->is_sound_need) { SoundEffect::PlaySoundEffect(SFX::Book_open);
        SoundEffect::SetSoundEffectVolume(SFX::Book_open, SOUND);
        }
    }
}

void portalbook_image::state_Open::Enter([[maybe_unused]] GameObject* object) {
    portalbook_image* image = static_cast<portalbook_image*>(object);
    if (image->is_game_book == true) {
        if (*image->now_book_clear == false) {
            image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Normal_Open));
        }
        else {
            image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Gold_Open));
        }
    }
    else if (image->is_mode_book == true) {
        image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(image->last_ani));
    }
}

void portalbook_image::state_Open::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
}

void portalbook_image::state_Open::CheckExit([[maybe_unused]] GameObject* object) {
    portalbook_image* image = static_cast<portalbook_image*>(object);
    if (image->is_open == false) {
        if (image->is_game_book == true) {
            if (*image->now_book_clear == false) {
                image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Normal_Close));
            }
            else {
                image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Gold_Close));
            }
        }
        else if (image->is_mode_book == true) {
            image->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(image->last_ani) + 1);
        }

        image->change_state(&image->state_idle);
        if (image->is_sound_need) {
            SoundEffect::PlaySoundEffect(SFX::Book_close);
            SoundEffect::SetSoundEffectVolume(SFX::Book_close, SOUND);
        }
    }
}
