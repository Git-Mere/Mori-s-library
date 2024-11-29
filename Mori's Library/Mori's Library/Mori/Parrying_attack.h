#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
    class Parrying_attack : public Particle {
    public:
        Parrying_attack();

        std::string TypeName() override { return "parrying attack"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::parrying_attack; }

        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 5;
        void* ptr = nullptr;
        int number = 0;

    private:
        enum class Animations {
            Idle,
            explode
        };

        bool set_velocity = false;
        class State_Run : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Run"; }
        };
        State_Run state_run;

    };
}