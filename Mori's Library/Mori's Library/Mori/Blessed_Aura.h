#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include "Player.h"

class Blessed_Aura : public GameObject {
public:
    Blessed_Aura(vec2 position, Player& player);
    GameObjectTypes Type() override { return GameObjectTypes::Blessed_Aura; }
    std::string TypeName() override { return "Blessed_Aura"; }

    void Draw(const mat3& world_to_ndc);

    class State_Invisible : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Invisible"; }
    };
    State_Invisible invisible;

    class State_Visible : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    State_Visible visible;

    class State_Ultimate : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Ultimate"; }
    };
    State_Ultimate ultimate;

private:
    Player* player_;
    vec2 playerpos = vec2({ 0, 0 });
    const vec2 Aurasize = vec2(130, 130);

    enum class Animations {
        Invisible,
        Visible,
        Ultimate
    };
};