#pragma once
#include "../Engine/GameObject.h"
#include "Player.h"

class portalbook_image;

class Scroll : public GameObject {
public:
    Scroll(vec2 position, std::string book_name, portalbook_image& portalbookimage);

    GameObjectTypes Type() override { return GameObjectTypes::Scroll; }
    std::string TypeName() override { return "Scroll"; }
    void Update(float dt) override;
    void Draw(const mat3& world_to_ndc);

    //Default
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

    //Open
    class state_Open : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Open"; }
    private:
        float timer = 0;
    };
    state_Open state_open;

private:
    enum class Animations {
        Idle,
        Open,
        close,
    };

    portalbook_image* portalbook_image_;
    vec2 real_position = vec2(0, 0);
    vec2 hide_position = vec2(3000, 3000);
};