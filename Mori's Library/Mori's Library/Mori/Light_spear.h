#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
    class Light_spear : public Particle {
    public:
        Light_spear();

        std::string TypeName() override { return "Light_spear"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::Light_spear; }

        static constexpr int MaxCount = 8;
        static constexpr double MaxLife = 5;
        void* ptr;
        int number;

    private:
        bool success_parrying = false;
        float scale = 150;
        class State_Run : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Run"; }
        };
        State_Run state_run;

    };


///////////////////////////////////// Dark_Spear  /////////////////////////////////////
    class Dark_Spear : public Particle {
    public:
        Dark_Spear();

        std::string TypeName() override { return "Dark Spear"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision([[maybe_unused]] GameObject* other_object);
        GameObjectTypes Type() override { return GameObjectTypes::Dark_Spear; }

        static constexpr int MaxCount = 16;
        static constexpr double MaxLife = 5;
        void* ptr;
        int number;

    private:
        bool success_parrying = false;
        float scale = 150;
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