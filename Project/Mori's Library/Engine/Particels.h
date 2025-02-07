#pragma once
#include "Particle.h"
#include <Random.h>

namespace Particles {
    //Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball Slime_ball 
    class Slime_ball : public Particle {
    public:
        Slime_ball() : Particle("assets/sprite/Book1/slime_attack.spt") {
            SetScale(vec2(120));
            SetCollisionScale(vec2(50), true);
        };
        GameObjectTypes Type() override { return GameObjectTypes::slime_ball; }
        std::string TypeName() override { return "Slime_ball"; }
        bool CanCollideWith(GameObjectTypes other_object_type) {
            if (other_object_type == GameObjectTypes::Player) {
                return true;
            }
            return false;
        };
        void ResolveCollision(GameObject* other_object)
        {
            Player* player = static_cast<Player*>(other_object);
            if (player->Get_parrying_mode()) {
                SetPosition({ -2000, -2000 });
            }
        }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 5.0;
    };
    class Smoke : public Particle {
    public:
        Smoke() : Particle("assets/sprite/Smoke.spt") {
            SetScale(vec2(50));
        };
        GameObjectTypes Type() override { return GameObjectTypes::Smoke; }
        std::string TypeName() override { return "Smoke"; }
        static constexpr int MaxCount = 11;
        static constexpr double MaxLife = 0.3;
    };

    class Flower_particle : public Particle {
    public:
        Flower_particle() : Particle("assets/sprite/Book4/flower_particle.spt") {
            SetScale(vec2(10));
        };
        GameObjectTypes Type() override { return GameObjectTypes::Flower_particle; }
        std::string TypeName() override { return "Flower particle"; }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 3.0;
    };

    class Blossomtree : public Particle {
    public:
        Blossomtree() : Particle("assets/sprite/Book4/Blossomtree.spt") {
            SetScale(vec2(500));
        };
        GameObjectTypes Type() override { return GameObjectTypes::Blossomtree; }
        std::string TypeName() override { return "Blossom tree"; }
        static constexpr int MaxCount = 1;
        static constexpr double MaxLife = 16.0;
    };

    class BossHit : public Particle {
    public:
        BossHit() : Particle("assets/sprite/Book4/BossHit.spt") {
            SetScale(vec2(300, 300));
        };
        GameObjectTypes Type() override { return GameObjectTypes::BossHit; }
        std::string TypeName() override { return "Boss Hit"; }
        static constexpr int MaxCount = 1;
        static constexpr double MaxLife = 1.0;
    };

    class Divine_light : public Particle {
    public:
        Divine_light() : Particle("assets/sprite/Bible/Divine_light.spt") {
            SetScale(vec2(200));
            SetCollisionScale(vec2{ 150 }, true);
        };
        GameObjectTypes Type() override { return GameObjectTypes::divine_light; }
        std::string TypeName() override { return "divine_light"; }
        bool CanCollideWith(GameObjectTypes other_object_type) {
            if (other_object_type == GameObjectTypes::Player)
            {
                return true;
            }
            return false;
        };
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 1.8;
    };
    class Explode : public Particle {
    public:
        Explode() : Particle("assets/sprite/Explode.spt") {
            SetScale(vec2(60));
        };
        GameObjectTypes Type() override { return GameObjectTypes::Explode; }
        std::string TypeName() override { return "explode"; }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 0.5;
    };

    class Boom : public Particle {
    public:
        Boom() : Particle("assets/sprite/Boom.spt") {
            SetScale(vec2(60));
            SetCollisionScale(vec2{ 60,60 }, true);
        };
        GameObjectTypes Type() override { return GameObjectTypes::Boom; }
        std::string TypeName() override { return "Boom"; }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 0.5;
    };

}
