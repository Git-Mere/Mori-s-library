#include "Tuto_ball.h"
#include "tuto_dummy.h"

void Particles::Tuto_ball::Update(float dt)
{
    Particle::Update(dt);
    if (Alive() == false) {
        success_dodge = false;
        success_parrying = false;
    }
}

bool Particles::Tuto_ball::CanCollideWith(GameObjectTypes other_object_type)
{
    if (other_object_type == GameObjectTypes::Player && Alive() == true) {
        return true;
    }
    return false;
}

bool Particles::Tuto_ball::Canparrying()
{
    tuto_dummy* dummy = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetObject<tuto_dummy>();
    if (dummy != nullptr && dummy->Get_mode() == Parrying) {
        return true;
    }
    return false;
}

void Particles::Tuto_ball::ResolveCollision(GameObject* other_object)
{
    Player* player = static_cast<Player*>(other_object);
    
    tuto_dummy* dummy = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetObject<tuto_dummy>();
    if (dummy != nullptr) {
        if (dummy->Get_mode() == Dodge) {
            if (player->Get_invin() == false && success_dodge == false) {
                success_dodge = true;
                dummy->count();
            }
        }
        else if (dummy->Get_mode() == Parrying) {
            if (player->Get_parrying_mode() && success_parrying == false) {
                success_parrying = true;
                SetVelocity(-vec2{ GetVelocity() });
                dummy->count();
            }
        }
    }
}
