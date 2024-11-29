#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/GameState.h"
#include "Player.h"
#include "Timer.h"

class Slime : public GameObject {
public:
    Slime() : Slime(nullptr, vec2(0, 0), *player_) {}
    Slime([[maybe_unused]] Slime* parent, vec2 position, Player& player);

    GameObjectTypes Type() override { return GameObjectTypes::Slime; }
    std::string TypeName() override { return "Slime"; }
    DownTimer* hurt_timer;
    DownTimer* attack_timer;

    Player* player_;
    const vec2 Return_coordinate();

    void Update(float dt)override;
    void Draw(const mat3& world_to_ndc);
    void update_velocity(float dt);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object) override;
    void modeSetting();
    bool Return_is_collide();

    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update(GameObject* slime, float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Idle"; }
    };

    State_Idle state_idle;

    class State_Running : public State {
    public:
        virtual void Enter(GameObject* slime) override;
        virtual void Update(GameObject* slime, float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Running"; }
    };

    State_Running state_running;


    class State_Long_Range_Attack : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Running"; }
    };

    State_Long_Range_Attack state_long;

    class Jump_attack : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Running"; }
    };

    Jump_attack state_jump;

    class Player_Down : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Player Down!"; }
    };

    Player_Down state_player_down;

private:
    vec2 move_direction = { 0 ,0 };
    vec2 scale = { 0, 0 };
    vec2 parent_position = { 0,0 };
    vec2 particle_vec2 = { 0,0 };

    float theta = 0;
    float current_slime_Yposition = 0;
    float health;
    float speed = 0;
    float hurt_time = 1.0f;
    float angle = 0;

    bool is_hurt = false;
    bool is_shoot = false;
    bool is_jump = false;
    bool is_collide = true;
    bool is_pattern_finished = false;
    bool stop_move = false;
    bool only_once = false;
    int size;
    short shooting_count = 0;
    double long_range_attack_delay = 0;

    int rotate_shot_count = 6;
    vec2 shot_speed{ -300, 0 };


    static constexpr int default_size = 2;
    static constexpr float default_healths[] = { 2.0f, 3.0f, 4.0f };
    static constexpr float default_scales[] = { 200.0f, 300.0f, 450.0f };
    static constexpr float default_speed[] = { 60.0f, 30.0f, 0.0f };
    static constexpr float acceleration = 50.0f;
    static constexpr float max_velocity = 100.0f;
    static constexpr float drag = 700.0f;

    enum class Animations {
        Idle,
        Fade,
        Shoot,
        jump_attack,
        randing_attack
    };

protected:
    vec2 playerPosition = vec2({ 0, 0 });
};