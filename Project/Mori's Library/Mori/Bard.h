#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/GameState.h"
#include "Player.h"
#include "Timer.h"
#include "Summoner.h"
#include "Electronic_wave.h"
class Bard : public GameObject {
public:
    Bard(vec2 position, Player& player);
    GameObjectTypes Type() override { return GameObjectTypes::Bard; }
    std::string TypeName() override { return "Bard"; }

    void Update(float dt)override;
    void Draw(const mat3& world_to_ndc);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object) override;
    const vec2 Return_coordinate();

    void update_velocity(float dt);

    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };

    State_Idle state_idle;

    class State_Running : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Running"; }
    };

    State_Running state_running;

    class State_Attack : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Attack"; }
    };

    State_Attack state_attack;

    class State_summons : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Meteor"; }
    };

    State_summons state_summon;

    class State_crime : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Meteor"; }
    };

    State_crime state_crime;

    class State_range : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Meteor"; }
    };

    State_range state_range;

    class Player_Down : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Player Down!"; }
    };

    Player_Down state_player_down;

    vec2 particle_vec2 = { 0, 0 };

private:
    DownTimer* hurt_timer;
    Player* player_;
    vec2 playerPosition = vec2({ 0, 0 });
    vec2 move_direction = { 0 ,0 };
    Summoner* summoner;

    vec2 missile_speed{ 1000, 0 };
    vec2 missile_speed2{ 600, 0 };
    float missile_delay = 0.4f;
    float laser_time = 1.f;

    const vec2 scale = vec2(500, 500);
    vec2 ColSize = vec2(70, 100);

    float hurt_time = 1.0f;
    float speed = 0;
    unsigned HP = 50;

    bool is_hurt = false;
    bool stop_move = false;
    bool is_collision = true;

    bool is_shoot = false;
    bool is_pattern_finished = false;
    short shooting_count = 0;
    double attack_delay = 0;
    float angle = 0;

    bool has_summon = false;

    static constexpr float acceleration = 50;
    static constexpr float max_velocity = 100;
    static constexpr float drag = 700;


    float time = 0;
    bool draw_electronic = false;
    bool draw_line = false;
    int randomX = 0;

    float x_position = 0;
    float y_position = 0;

    enum class Animations {
        Idle,
        Running,
        Attack,
        Crime,
        Range
    };
};