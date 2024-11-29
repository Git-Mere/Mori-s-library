#include "Javelin.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Particels.h"
#include "Lucifer.h"
#include "Fallen_Lucifer.h"

namespace Particles {
    Javelin::Javelin() : Particle("assets/sprite/Bible/Light_spear.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(100, 50));
        SetCollisionScale(vec2{ 100,25 }, true);
    };

    bool Javelin::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void Javelin::ResolveCollision(GameObject* other_object)
    {
        Player* player = static_cast<Player*>(other_object);
        if (player->Get_parrying_mode()) {
            SetPosition(vec2(-5000, -5000));
            vec2 collisionPoint = other_object->GetPosition();
            vec2 tmp = normalize(GetVelocity());
            vec2 velocity_and_direction = vec2(tmp.x * -100, tmp.y * -80);
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Explode>>()
                ->Emit(1, collisionPoint, velocity_and_direction, velocity_and_direction, Math::PI/3);
        }
    }

 ///////////////////////////////////// Dark_Javelin(pattern)  /////////////////////////////////////

    Dark_Javelin::Dark_Javelin() : Particle("assets/sprite/Bible/Dark_spear.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(100, 50));
        SetCollisionScale(vec2{ 100,25 }, true);
    };

    bool Dark_Javelin::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void Dark_Javelin::ResolveCollision(GameObject* other_object)
    {
        Player* player = static_cast<Player*>(other_object);
        if (player->Get_parrying_mode()) {
            SetPosition(vec2(-5000, -5000));
            vec2 collisionPoint = other_object->GetPosition();
            vec2 tmp = normalize(GetVelocity());
            vec2 velocity_and_direction = vec2(tmp.x * -100, tmp.y * -80);
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Explode>>()
                ->Emit(1, collisionPoint, velocity_and_direction, velocity_and_direction, Math::PI / 3);
        }
    }
}