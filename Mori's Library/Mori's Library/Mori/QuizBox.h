#pragma once
#include "../Engine/GameObject.h"
#include "Random.h"

class QuizBox : public GameObject {
public:
    QuizBox(vec2 position, bool* ptr, int button_number);//
    void Update([[maybe_unused]] float dt);
    GameObjectTypes Type() override { return GameObjectTypes::QuizBox; }
    std::string TypeName() override { return "QuizBox"; }
    bool CanCollideWith(GameObjectTypes other_object_type) override;
    void ResolveCollision(GameObject* other_object) override;
    void SetBoxScale(vec2 scale);
private:
    enum class Animations {
        Pressed,
        Up
    };

    //Pressed
    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    State_Idle state_idle;

    //Pressed
    class State_Pressed : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Pressed"; }
    };
    State_Pressed state_pressed;

    ////Up
    //class State_Up : public State {
    //public:
    //    virtual void Enter(GameObject* object) override;
    //    virtual void Update(GameObject* object, float dt) override;
    //    virtual void CheckExit(GameObject* object) override;
    //    std::string GetName() override { return "Up"; }
    //};
    //State_Up state_up;

    float box_x = Engine::Instance()->Get_Window().x / 3;
    float box_y = Engine::Instance()->Get_Window().y / 3;
    bool* answered = nullptr;
};
