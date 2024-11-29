#include "Parrying_attack.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Particels.h"
#include "Bard.h"
#include "Javelin.h"
namespace Particles {
    Parrying_attack::Parrying_attack() : Particle("assets/sprite/Book5/missile.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(50, 100));
        SetCollisionScale(vec2{ 100,25 }, true);
        current_state = &state_run;
        current_state->Enter(this);
        number = 0;
        ptr = nullptr;
    };

    bool Parrying_attack::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Bard) {
            return true;
        }
        return false;
    };

    void Parrying_attack::ResolveCollision([[maybe_unused]] GameObject* other_object)
    {
        this->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::explode));
        this->SetVelocity(vec2{ 0, 0 });
    }

    void Parrying_attack::State_Run::Enter([[maybe_unused]] GameObject* object) {//
        Parrying_attack* tone = static_cast<Parrying_attack*>(object);
        tone->set_velocity = false;
    }

    void Parrying_attack::State_Run::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        Parrying_attack* tone = static_cast<Parrying_attack*>(object);

        if (tone->set_velocity == false) {
            tone->SetVelocity(reinterpret_cast<Player*>(tone->ptr)->particle_vec2);
            tone->set_velocity = true;
        }

        if (tone->GetGOComponent<Sprite>()->AnimationEnded())
        {
            //tone->GetGOComponent<Sprite>()->ResetAnimation();
            tone->SetPosition({ -5000, -5000 });
            tone->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));

            //cosmic_tone->Destroy();
        }
    }

    void Parrying_attack::State_Run::CheckExit([[maybe_unused]] GameObject* object) {

    }
}