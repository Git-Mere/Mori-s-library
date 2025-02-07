#include "Cosmic_tone.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Particels.h"
#include "Bard.h"
#include "Parrying_attack.h"

namespace Particles {
    Cosmic_tone::Cosmic_tone() : Particle("assets/sprite/Book5/missile.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(50, 100));
        SetCollisionScale(vec2{ 25,100 }, true);
        current_state = &state_run;
        current_state->Enter(this);
        number = 0;
        ptr = nullptr;
        is_shoot = true;
    };

    bool Cosmic_tone::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void Cosmic_tone::ResolveCollision(GameObject* other_object)
    {
        Player* player = static_cast<Player*>(other_object);

        if (player->Get_parrying_mode()) {
            vec2 playerPosition = player->GetPosition();
            vec2 relativePosition = reinterpret_cast<Bard*>(this->ptr)->GetPosition() - playerPosition;
            float angle = atan2(relativePosition.y, relativePosition.x);

            player->particle_vec2 = Math::RotationMatrix(angle) * vec2 { 1200, 0 };
            //if (is_shoot == true) {
                Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::Parrying_attack>>()->Rotate_Shot
                (1, vec2{ player->GetPosition() }, vec2{ player->particle_vec2 }, angle - Math::PI / 2);
                is_shoot = false;
                success_parrying = true;
                player->combo_damage();
            //}
        }
        else
        {
            this->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::explode));
            this->SetVelocity(vec2{ 0, 0 });
        }

        //if (this->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::explode))
        //{
        //    if (this->GetGOComponent<Sprite>()->AnimationEnded())
        //    {
        //        this->SetPosition({ -5000, -5000 });
        //    }
        //}
        //this->Destroy();
    }

    void Cosmic_tone::State_Run::Enter([[maybe_unused]] GameObject* object) {//
 
    }

    void Cosmic_tone::State_Run::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        Cosmic_tone* cosmic_tone = static_cast<Cosmic_tone*>(object);
        //if (cosmic_tone->GetGOComponent<Sprite>()->CurrentAnimation() == static_cast<int>(Animations::explode))
        {
            //if (cosmic_tone->success_parrying)
            //{
            //    cosmic_tone->SetPosition({ -5000, -5000 });
            //}
            if (cosmic_tone->GetGOComponent<Sprite>()->AnimationEnded())
            {
                cosmic_tone->SetPosition({ -5000, -5000 });
                cosmic_tone->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
                cosmic_tone->is_shoot = true;
                //cosmic_tone->Destroy();
            }
        }
    }

    void Cosmic_tone::State_Run::CheckExit([[maybe_unused]] GameObject* object) {

    }
}