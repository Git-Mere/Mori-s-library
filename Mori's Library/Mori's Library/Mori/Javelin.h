#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
    class Javelin : public Particle {
    public:
        Javelin();

        std::string TypeName() override { return "javelin"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::javelin; }

        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 5;
        void* ptr = nullptr;
        int number;
        
    private:

    };
    ///////////////////////////////////// Dark_Javelin(pattern)  /////////////////////////////////////

    class Dark_Javelin : public Particle {
    public:
        Dark_Javelin();

        std::string TypeName() override { return "Dark Javelin"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::Dark_Javelin; }

        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 5;
        void* ptr;
        int number;

    private:


    };
}