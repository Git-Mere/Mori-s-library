#include "Light_spear.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Particels.h"
#include "Lucifer.h"
#include "Fallen_Lucifer.h"

namespace Particles {
    Light_spear::Light_spear() : Particle("assets/sprite/Bible/Light_spear.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(scale, scale / 3 * 2));
        SetCollisionScale(vec2{ scale, scale / 6 }, true);
        current_state = &state_run;
        current_state->Enter(this);
    };
    bool Light_spear::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void Light_spear::ResolveCollision(GameObject* other_object)
    {
        Player* player = static_cast<Player*>(other_object);
        if (player->Get_parrying_mode()) {
            SetPosition({ -5000, -5000 });
            vec2 collisionPoint = other_object->GetPosition();
            vec2 tmp = normalize(GetVelocity());
            vec2 velocity_and_direction = vec2(tmp.x * -100, tmp.y * -80);
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Explode>>()
                ->Emit(1, collisionPoint, velocity_and_direction, velocity_and_direction, Math::PI/3);

            success_parrying = true;
        }
    }

    void Light_spear::State_Run::Enter([[maybe_unused]] GameObject* object) {
        Light_spear* spear = static_cast<Light_spear*>(object);
        spear->success_parrying = false;
    }

    void Light_spear::State_Run::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        Light_spear* spear = static_cast<Light_spear*>(object);

        if (reinterpret_cast<Lucifer*>(spear->ptr)->ReturnParticleMove()) {
            spear->SetVelocity(reinterpret_cast<Lucifer*>(spear->ptr)->particle_vec[spear->number]);
        }

    }

    void Light_spear::State_Run::CheckExit([[maybe_unused]] GameObject* object) {

    }

///////////////////////////////////// Dark_Spear  /////////////////////////////////////
    Dark_Spear::Dark_Spear() : Particle("assets/sprite/Bible/Dark_spear.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(scale, scale / 3 * 2));
        SetCollisionScale(vec2{ scale, scale / 6 }, true);
        current_state = &state_run;
        current_state->Enter(this);
    };
    bool Dark_Spear::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void Dark_Spear::ResolveCollision(GameObject* other_object)
    {
        Player* player = static_cast<Player*>(other_object);
        if (player->Get_parrying_mode()) {
            SetPosition({ -5000, -5000 });
            vec2 collisionPoint = other_object->GetPosition();
            vec2 tmp = normalize(GetVelocity());
            vec2 velocity_and_direction = vec2(tmp.x * -100, tmp.y * -80);
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Explode>>()
                ->Emit(1, collisionPoint, velocity_and_direction, velocity_and_direction, Math::PI / 3);

            success_parrying = true;
        }
    }

    void Dark_Spear::State_Run::Enter([[maybe_unused]] GameObject* object) {
        Dark_Spear* spear = static_cast<Dark_Spear*>(object);
        spear->success_parrying = false;
    }

    void Dark_Spear::State_Run::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        Dark_Spear* spear = static_cast<Dark_Spear*>(object);

        if (reinterpret_cast<Fallen_Lucifer*>(spear->ptr)->ReturnParticleMove()) {
            spear->SetVelocity(reinterpret_cast<Fallen_Lucifer*>(spear->ptr)->particle_vec[spear->number]);
        }

    }

    void Dark_Spear::State_Run::CheckExit([[maybe_unused]] GameObject* object) {

    }

}