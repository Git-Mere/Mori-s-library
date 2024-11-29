#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"
#include <Random.h>

namespace Particles {
    class FinalSpark : public Particle {
    public:
        FinalSpark();
       // void SetLucifer(std::shared_ptr<Lucifer> lucifer);
        std::string TypeName() override { return "FinalSpark"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::FinalSpark; }

        static constexpr int MaxCount = 4;
        static constexpr double MaxLife = 10.0;
        void* ptr;
        int number;
    private:
        float theta = 0;
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
            float timer = 0;
        };
        State_Ready state_ready;

        class State_Run : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Run"; }
        private:
            float timer = 0;
        };
        State_Run state_run;

    };
/////////////////////////////////////////////////////// Dark Spark ///////////////////////////////////////////////////////
    class DarkSpark : public Particle {
    public:
        DarkSpark();
        std::string TypeName() override { return "Dark Spark"; }
        //void SetCollision(bool is_colli) { is_collision = is_colli; }
        //bool GetCanCollision() { return is_collision; }
        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::DarkSpark; }

        static constexpr int MaxCount = 8;
        static constexpr double MaxLife = 10.0;
        void* ptr;
        int number;
    private:
        float theta = 0;
        //bool is_collision = false;

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
            float timer = 0;
        };
        State_Ready state_ready;

        class State_Run : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Run"; }
        private:
            float timer = 0;
        };
        State_Run state_run;

    };

/////////////////////////////////////// Lazer ///////////////////////////////////////
    class Lazer : public Particle {
    public:
        Lazer();
        std::string TypeName() override { return "Lazer"; }
        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::Lazer; }

        static constexpr int MaxCount = 1;
        static constexpr double MaxLife = 1.0;

        enum class Animations {
            Ready
        };

    };
}