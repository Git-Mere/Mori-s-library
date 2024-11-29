#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
    class Electronic : public Particle {
    public:
        Electronic();

        std::string TypeName() override { return "cosmic tone"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        //void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::electronic; }

        static constexpr int MaxCount = 1;
        static constexpr double MaxLife = 1.9;
        void* ptr = nullptr;
        int number = 0;
    };

    class Electronic_line : public Particle {
    public:
        Electronic_line();

        std::string TypeName() override { return "cosmic tone"; }

        //bool CanCollideWith(GameObjectTypes other_object_type) override;
        //void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::electronic_line; }

        static constexpr int MaxCount = 1;
        static constexpr double MaxLife = 0.10;
        void* ptr = nullptr;
        int number = 0;
    };
}