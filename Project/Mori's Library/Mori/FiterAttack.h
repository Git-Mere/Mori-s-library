#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Particle.h"

namespace Particles {
//////////////////////////////////////// Energy Kick ////////////////////////////////////////
    class Energy_Kick1 : public Particle {
    public:
        Energy_Kick1();
        GameObjectTypes Type() override { return GameObjectTypes::Energy_Kick; }
        std::string TypeName() override { return "Energy Kick"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;

        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 20.0f;
    };
    class Energy_Kick2 : public Particle {
    public:
        Energy_Kick2();
        GameObjectTypes Type() override { return GameObjectTypes::Energy_Kick; }
        std::string TypeName() override { return "Energy Kick"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;

        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 20.0f;
    };
//////////////////////////////////////// Energy Fist ////////////////////////////////////////
    class Energy_Fist : public Particle {
    public:
        Energy_Fist();
        GameObjectTypes Type() override { return GameObjectTypes::Energy_Fist; }
        std::string TypeName() override { return "Energy Fist"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;
        void ResolveCollision(GameObject* other_object);
        
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 10.0f;
    };

//////////////////////////////////////// Energy Lazer ////////////////////////////////////////
    class Energy_Lazer_L : public Particle {
    public:
        Energy_Lazer_L();
        std::string TypeName() override { return "Energy Lazer"; }
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }
        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::Energy_Lazer; }

        static constexpr int MaxCount = 3;
        static constexpr double MaxLife = 3.0;

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
            static constexpr float ready_time = 0.15f;   // generator time
        };
        State_Ready state_ready;


        class State_Shoot : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Shoot"; }
        private:
            float count2 = 0;
        };
        State_Shoot state_shoot;
    };
    //R
    class Energy_Lazer_R : public Particle {
    public:
        Energy_Lazer_R();
        std::string TypeName() override { return "Energy Lazer"; }
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }
        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::Energy_Lazer; }

        static constexpr int MaxCount = 3;
        static constexpr double MaxLife = 3.0;

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
            static constexpr float ready_time = 0.15f;   // generator time
        };
        State_Ready state_ready;


        class State_Shoot : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Shoot"; }
        private:
            float count2 = 0;
        };
        State_Shoot state_shoot;
    };

//////////////////////////////////////// PunchEarthquake ////////////////////////////////////////
    class PunchEarthquake : public Particle {
    public:
        PunchEarthquake();
        GameObjectTypes Type() override { return GameObjectTypes::PunchEarthquake; }
        std::string TypeName() override { return "Punch Earthquake"; }

        bool CanCollideWith(GameObjectTypes other_object_type) override;

        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 15.0f;
    };

//////////////////////////////////////// Energy Lazer (Big) ////////////////////////////////////////
    class B_Energy_Lazer_L : public Particle {
    public:
        B_Energy_Lazer_L();
        std::string TypeName() override { return "Energy Lazer"; }
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }
        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::Energy_Lazer; }

        static constexpr int MaxCount = 3;
        static constexpr double MaxLife = 3.0;

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
            static constexpr float ready_time = 0.15f;   // generator time
        };
        State_Ready state_ready;


        class State_Shoot : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Shoot"; }
        private:
            float count2 = 0;
        };
        State_Shoot state_shoot;
    };
    //R
    class B_Energy_Lazer_R : public Particle {
    public:
        B_Energy_Lazer_R();
        std::string TypeName() override { return "Energy Lazer"; }
        void SetCollision(bool is_colli) { is_collision = is_colli; }
        bool GetCanCollision() { return is_collision; }
        bool CanCollideWith(GameObjectTypes other_object_type) override;

        GameObjectTypes Type() override { return GameObjectTypes::Energy_Lazer; }

        static constexpr int MaxCount = 3;
        static constexpr double MaxLife = 3.0;

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
            static constexpr float ready_time = 0.15f;   // generator time
        };
        State_Ready state_ready;


        class State_Shoot : public State {
        public:
            virtual void Enter(GameObject* object) override;
            virtual void Update([[maybe_unused]] GameObject* object, float dt) override;
            virtual void CheckExit(GameObject* object) override;

            std::string GetName() override { return "Shoot"; }
        private:
            float count2 = 0;
        };
        State_Shoot state_shoot;
    };


}
