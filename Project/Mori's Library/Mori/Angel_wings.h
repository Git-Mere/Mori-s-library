#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include "Lucifer.h"

class Angel_wings : public GameObject {
public:
    Angel_wings(vec2 position, Lucifer& lucifer);
    GameObjectTypes Type() override { return GameObjectTypes::Angel_wings; }
    std::string TypeName() override { return "Angel_wings"; }

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
    Lucifer* lucifer_;
    vec2 luciferPosition = vec2({ 0, 0 });
    const vec2 WingsSize = vec2(400, 400);

    enum class Animations {
        Visible,
        Invisible
    };
};