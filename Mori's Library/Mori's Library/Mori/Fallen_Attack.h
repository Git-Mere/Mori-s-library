#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
    class Fire_Pillar : public Particle {
    public:
        Fire_Pillar();
        std::string TypeName() override { return "Fire Pillar"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::Fire_Pillar; }

        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 20;

    private:
    };

//////////////////////////////////////////////////////////// Fire Pit ////////////////////////////////////////////////////////////
    class Fire_Pit : public Particle {
    public:
        Fire_Pit();
        std::string TypeName() override { return "Fire Pit"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }

        GameObjectTypes Type() override { return GameObjectTypes::Fire_Pit; }

        static constexpr int MaxCount = 60;
        static constexpr double MaxLife = 2.0;

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
            static constexpr float ready_time = static_cast<float>(MaxLife) / 2.0f;   // generator time
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

//////////////////////////////////////////////////////////// Divine Dark ////////////////////////////////////////////////////////////
    class Divine_Dark : public Particle {
    public:
        Divine_Dark() : Particle("assets/sprite/Bible/Divine_Dark.spt") {
            SetScale(vec2(350));
            SetCollisionScale(vec2{ 250 }, true);
        };
        GameObjectTypes Type() override { return GameObjectTypes::Divine_Dark; }
        std::string TypeName() override { return "Divine Dark"; }
        bool CanCollideWith(GameObjectTypes other_object_type) {
            if (other_object_type == GameObjectTypes::Player)
            {
                return true;
            }
            return false;
        };
        static constexpr int MaxCount = 40;
        static constexpr double MaxLife = 1.8;
    };

//////////////////////////////////////////////////////////// Divine Dark2 ////////////////////////////////////////////////////////////
    class Divine_Dark2 : public Particle {
    public:
        Divine_Dark2() : Particle("assets/sprite/Bible/Divine_Dark.spt") {
            SetScale(vec2(-350, 350));
            SetCollisionScale(vec2{ 250 }, true);
        };
        GameObjectTypes Type() override { return GameObjectTypes::Divine_Dark; }
        std::string TypeName() override { return "Divine Dark"; }
        bool CanCollideWith(GameObjectTypes other_object_type) {
            if (other_object_type == GameObjectTypes::Player)
            {
                return true;
            }
            return false;
        };
        static constexpr int MaxCount = 20;
        static constexpr double MaxLife = 1.8;
    };


}