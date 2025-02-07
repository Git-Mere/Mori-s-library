#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"
namespace Particles {
    class Tuto_ball : public Particle {
    public:
        Tuto_ball() : Particle("assets/sprite/Tuto/Tuto_attack.spt") {
            SetScale(vec2(120));
            SetCollisionScale(vec2(60), true);
        };
        void Update(float dt) override;
        GameObjectTypes Type() override { return GameObjectTypes::Tuto_ball; }
        std::string TypeName() override { return "Tuto_ball"; }
        bool CanCollideWith(GameObjectTypes other_object_type);
        bool Canparrying();
        void ResolveCollision(GameObject* other_object);


        bool success_dodge = false;
        bool success_parrying = false;
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 10.;
    };

}