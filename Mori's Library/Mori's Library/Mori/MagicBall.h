#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"
#include <Random.h>

namespace Particles {
    class MagicBall : public Particle {
    public:
        MagicBall();
        std::string TypeName() override { return "MagicBall"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }

        GameObjectTypes Type() override { return GameObjectTypes::MagicBall; }

        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 5.4f;

    private:
        bool is_collision = false;
    private:
        enum class Animations {
            Ready,
            Boom
        };

        class State_Ready : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Ready"; }
        private:
            float count1 = 0;
            static constexpr float ready_time = MaxLife / 2;   // generator time
        };
        State_Ready state_ready;


        class State_Boom : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Boom"; }
        private:
            float count2 = 0;
        };
        State_Boom state_boom;
    };


    class Earthquake : public Particle {
    public:
        Earthquake();
        std::string TypeName() override { return "Earthquake"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }

        GameObjectTypes Type() override { return GameObjectTypes::Earthquake; }

        static constexpr int MaxCount = 1;
        static constexpr double MaxLife = 1.5;

    private:
        bool is_collision = false;
    private:
        enum class Animations {
            Ready,
            Boom
        };

        class State_Ready : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Ready"; }
        private:
            float count1 = 0;
        };
        State_Ready state_ready;


        class State_Boom : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Boom"; }
        private:
            float count2 = 0;
        };
        State_Boom state_boom;
    };
}