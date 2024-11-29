#pragma once
#include "../Engine/GameObject.h"
#include "../Mori/Timer.h"
#include "Player.h"
#include "book_scroll.h"

extern bool book1_clear[2];
extern bool book2_clear;
extern bool book3_clear[2];
extern bool book4_clear[2];
extern bool book5_clear[2];
extern bool bible_clear[2];

class portalbook_image : public GameObject {
public:
    portalbook_image(vec2 position, std::string book_name);

    GameObjectTypes Type() override { return GameObjectTypes::Portalbook_image; }
    std::string TypeName() override { return "portalbook_image"; }
    void Update(float dt) override;
    void Draw(const mat3& world_to_ndc);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object);
    void ChangePosition(vec2 position);
    bool Get_is_open() { return is_open; }
    void Check_Clear(std::string book_name);

    // Default
    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    private:
        bool onetime = false;
    };
    State_Idle state_idle;

    // Open
    class state_Open : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Open"; }
    };
    state_Open state_open;

private:
    Scroll* scroll_ptr = nullptr;
    bool is_open = false;
    bool is_sound_need = true;
    bool is_mode_book = false;
    bool is_game_book = false;
    bool is_book_clear[2];
    bool* now_book_clear = nullptr;
    std::string BookName;

    enum class Animations {
        Idle,
        Open,
        Close,
        HardOpen,
        HardClose,
        Glod
    };
    static const Animations Normal_Idle = Animations::Idle;
    static const Animations Normal_Open = Animations::Open;
    static const Animations Normal_Close = Animations::Close;
    static const Animations Gold_Idle = Animations::HardOpen;
    static const Animations Gold_Open = Animations::HardClose;
    static const Animations Gold_Close = Animations::Glod;

    Animations last_ani = Animations::Open;
};