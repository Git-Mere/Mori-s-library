#include "QuizBox.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include <Random.h>
#include "angles.h"
#include <iostream>

QuizBox::QuizBox(vec2 position, bool* ptr, int button_number) {
    switch (button_number)
    {
    case 1:
        AddGOComponent(new Sprite("assets/sprite/Book2/Button1.spt", this));
        break;
    case 2:
        AddGOComponent(new Sprite("assets/sprite/Book2/Button2.spt", this));
        break;
    case 3:
        AddGOComponent(new Sprite("assets/sprite/Book2/Button3.spt", this));
        break;
    case 4:
        AddGOComponent(new Sprite("assets/sprite/Book2/Button4.spt", this));
        break;
    default:
        AddGOComponent(new Sprite("assets/sprite/Book2/Button1.spt", this));
        break;
    }
    SetPosition({ position.x, position.y });
    SetScale(vec2(box_x, box_y));
    SetCollisionScale(vec2(box_x * 0.8f, box_y * 0.63f), true);
    answered = ptr;
    *answered = false;
    GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Up));
    current_state = &state_idle;
    current_state->Enter(this);
}

void QuizBox::Update([[maybe_unused]] float dt)
{
    GameObject::Update(dt);
}

bool QuizBox::CanCollideWith(GameObjectTypes other_object_type)
{
    if (other_object_type == GameObjectTypes::Player) {
        return true;
    }
    *answered = false;
    return false;
}

void QuizBox::ResolveCollision(GameObject* other_object) {
    switch (other_object->Type()) {
    case GameObjectTypes::Player:
        *answered = true;
        break;
    default:
        break;
    }
}

void QuizBox::SetBoxScale(vec2 scale) {
    SetScale(scale);
    SetCollisionScale(vec2(scale.x * 0.8f, scale.y * 0.8f), true);
}

//animation
void QuizBox::State_Idle::Enter([[maybe_unused]] GameObject* object) {
}

void QuizBox::State_Idle::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void QuizBox::State_Idle::CheckExit(GameObject* object) {
    QuizBox* quizbox = static_cast<QuizBox*>(object);
    if (quizbox->answered != nullptr && *quizbox->answered == true) {
        quizbox->change_state(&quizbox->state_pressed);
    }
}

//
void QuizBox::State_Pressed::Enter([[maybe_unused]] GameObject* object) {
    QuizBox* quizbox = static_cast<QuizBox*>(object);
    quizbox->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Pressed));
}

void QuizBox::State_Pressed::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {

}

void QuizBox::State_Pressed::CheckExit(GameObject* object) {
    QuizBox* quizbox = static_cast<QuizBox*>(object);
    if (quizbox->answered != nullptr && *quizbox->answered == false) {
        quizbox->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Up));
        quizbox->change_state(&quizbox->state_idle);
    }
}