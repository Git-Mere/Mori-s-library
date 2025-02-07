#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
	class Cosmic_tone : public Particle {
    public:
        Cosmic_tone();

        std::string TypeName() override { return "cosmic tone"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::cosmic_tone; }


        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 5;
        void* ptr = nullptr;
        int number = 0;

    private:
        enum class Animations {
            Idle,
            explode
        };

        class State_Run : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Run"; }
        };
        State_Run state_run;
        bool is_shoot = true;
        int num = 0;
        bool success_parrying = false;
    };
}