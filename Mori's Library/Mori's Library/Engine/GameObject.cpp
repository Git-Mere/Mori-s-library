#include "GameObject.h"
#include "Collision.h"
#include "ShowCollision.h"
#include "Engine.h"
#include <iostream>
#include "CameraCom.h"

GameObject::GameObject()
{
    current_state = &state_none;
}

GameObject::~GameObject() {
}

GameObject::GameObject(vec2 position) :
    GameObject(position, 0, { 1, 1 })
{
    current_state = &state_none;
}

GameObject::GameObject(vec2 position, float rotation, vec2 scale) :
    velocity({ 0,0 }),
    position(position),
    scaling(scale),
    angle_disp(rotation)
{
    current_state = &state_none;
}

bool GameObject::IsCollidingWith(GameObject* other_object) {
    Collision* collider = GetGOComponent<Collision>();
    if (collider != nullptr) {
        return collider->IsCollidingWith(other_object);
    }
    return 0;
}

bool GameObject::CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type) {
    return false;
}


void GameObject::Destroy()
{
    destroy = true;
}

bool GameObject::Destroyed()
{
    return destroy;
}


void GameObject::Update(float dt) {
    current_state->Update(this, dt);
    UpdateGOComponents(dt);

    if (velocity.x != 0 || velocity.y != 0) {
        UpdatePosition(velocity * dt);
    }
    current_state->CheckExit(this);
}

void GameObject::change_state(State* new_state) {
    Engine::GetLogger().LogEvent(current_state->GetName() + new_state->GetName());
    current_state = new_state;
    current_state->Enter(this);
}


void GameObject::Draw(const mat3& world_to_ndc) {
    Sprite* sprite = GetGOComponent<Sprite>();

    if (sprite != nullptr) {
        sprite->Draw();
    }
    Collision* collision = GetGOComponent<Collision>();
    ShowCollision* collision2 = Engine::GetGameStateManager().GetGSComponent<ShowCollision>();
    if (collision != nullptr && collision2 != nullptr) {
        if (collision2->Enabled()) {
            collision->Draw(world_to_ndc);
        }
    }
}

void GameObject::decide_matrix(const mat3& world_to_ndc)
{
    mat3 T = mat3::build_translation(GetPosition() + translation_pivot);
    mat3 T2 = mat3::build_translation(GetPosition());
    mat3 R = mat3{ cos(GetRotation()), sin(GetRotation()),0 ,-sin(GetRotation()) ,cos(GetRotation()),0,0,0,1 };
    mat3 H = mat3::build_scale(GetScale());
    if (rotation_pivot != vec2()) {
        R = mat3::build_translation(rotation_pivot) * R * mat3::build_translation(-rotation_pivot);
    }

    if (different_collision == true) {
        const mat3 H2 = mat3::build_scale(GetCollisionScale());
        model_to_ndc_for_collision = T2 * R * H2;
    }


    model_to_ndc = T * R * H;
    final_matrix = world_to_ndc * model_to_ndc;
}


const mat3& GameObject::GetMatrix() 
{
    if (different_collision) {
        return model_to_ndc_for_collision;
    }
    return model_to_ndc;
}

const mat3& GameObject::GetFinalMatrix()
{
    return final_matrix;
}

const vec2& GameObject::GetPosition() const
{
    return position;
}

const vec2& GameObject::GetVelocity() const
{
     return velocity;
}

const vec2& GameObject::GetScale() const
{
    return scaling;
}

const float GameObject::GetRotation() const
{
    return angle_disp;
}

void GameObject::SetPosition(vec2 new_position) {
    position = new_position;
}

void GameObject::SetTranslation_Pivot(vec2 value)
{
    translation_pivot = value;
}

void GameObject::UpdatePosition(vec2 delta) {
    position += delta;
}

void GameObject::SetVelocity(vec2 new_position)
{
    velocity = new_position;
}

void GameObject::UpdateVelocity(vec2 delta)
{
    velocity += delta;
}

void GameObject::SetScale(vec2 new_scale)
{
    scaling = new_scale;
}

void GameObject::UpdateScale(vec2 delta)
{
    scaling += delta;
}

void GameObject::SetRotation(float new_rotation)
{
    angle_disp = new_rotation;
}

void GameObject::SetRotation_Pivot(vec2 value)
{
    rotation_pivot = value;
}

void GameObject::UpdateRotation(float delta)
{
    angle_disp += delta;
}

void GameObject::SetCollisionScale(vec2 Collision_scale, bool is_chage) {
    different_collision = is_chage;
    collision_scaling = Collision_scale;
}

void GameObject::SetShaderName(Engine::ShaderName input)
{
    shadername = input;
}

void GameObject::Setcolor(vec4 input)
{
    color = input;
}


const vec2& GameObject::GetCollisionScale() const {
    if (different_collision) {
        return collision_scaling;
    }
    else {
        return GetScale();
    }
}

const Engine::ShaderName GameObject::GetShaderName() const
{
    return shadername;
}

const vec4& GameObject::Getcolor() const
{
    return color;
}
