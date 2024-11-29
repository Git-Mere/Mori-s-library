#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include "Fallen_Lucifer.h"

class Fallen_Angel_wings : public GameObject {
public:
    Fallen_Angel_wings(vec2 position, Fallen_Lucifer& lucifer);
    GameObjectTypes Type() override { return GameObjectTypes::Fallen_Angel_wings; }
    std::string TypeName() override { return "Fallen_Angel_wings"; }

    void Draw(const mat3& world_to_ndc);

    class State_Visible : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    State_Visible visible;

    class State_Invisible : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    State_Invisible invisible;


private:
    Fallen_Lucifer* fallen_lucifer_;
    vec2 luciferPosition = vec2({ 0, 0 });
    const vec2 WingsSize = vec2(400, 400);

    enum class Animations {
        Visible,
        Invisible
    };
};