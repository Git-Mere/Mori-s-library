#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include "Timer.h"
#include "player.h"
#include "../Engine/Constant.h"

class Fiter : public GameObject {
public:
    Fiter(vec2 position, Player& player);
    GameObjectTypes Type() override { return GameObjectTypes::Fiter; }
    std::string TypeName() override { return "Fiter"; }

    void Update(float dt)override;
    void Draw(const mat3& world_to_ndc);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object) override;
    float ReturnIsAnimationEnded() { return is_animation_ended; }

    void update_velocity(float dt);
    vec2 GetColSize() const { return BladeColSize; }
    bool GetBarrier() { return barrier; }
    void BarrierHited() { barrier_hited++; }
    void GetDamage();
    vec2 GetSize() { return BladeSize; }
    int GetHP() { return HP; }
    bool Return_is_collide() { return is_collide; }

private:
    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    State_Idle state_idle;

    //move
    class State_Running : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Running"; }
    };
    State_Running state_running;

    //wait
    class State_Waitting : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Waitting"; }
    private:
        int pattern_number = 0;
        double delay_time = 0;
    };
    State_Waitting state_waitting;

    //dash attack
    class State_Dash_Attack : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Dash Attack"; }
    private:
        float Dash_Speed = 1000;
        double Dash_Time = 0;
        vec2 direction = vec2(0, 0);
        bool animation_ontime = false;
        bool ontime = false;
    };
    State_Dash_Attack state_dash_attack;

    //energy kick
    class State_Energy_Kick : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* blade) override;
        virtual void Update([[maybe_unused]] GameObject* blade, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* blade) override;
        std::string GetName() override { return "Energy Kick"; }
    private:
        bool onetime = false;
        int count = 0;
        bool count_start = false;
    };
    State_Energy_Kick state_energy_kick;

    //energy fist
    class State_Energy_Fist : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* blade) override;
        virtual void Update([[maybe_unused]] GameObject* blade, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* blade) override;
        std::string GetName() override { return "Energy Fist"; }
    private:
        bool onetime = false;
        int create_count = 0;
        float pre_angle = 0;
        float orbitX = 0;
        float orbitY = 0;
    };
    State_Energy_Fist state_energy_fist;

    //Earthquake
    class State_Earthquake : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* blade) override;
        virtual void Update([[maybe_unused]] GameObject* blade, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* blade) override;
        std::string GetName() override { return "Earthquake"; }
    private:
        bool animation_check = false;
        bool onetime = false;
        float nermber = 3;
        float offsetX = 100.0f;
        float offsetY = 100.0f;
        vec2 topLeft =     vec2(-offsetX, -offsetY);
        vec2 topRight =    vec2(offsetX, -offsetY);
        vec2 bottomLeft =  vec2(-offsetX, offsetY);
        vec2 bottomRight = vec2(offsetX, offsetY);
    };
    State_Earthquake state_earthquake;

    //Energy lazer
    class State_Lazer : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* blade) override;
        virtual void Update([[maybe_unused]] GameObject* blade, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* blade) override;
        std::string GetName() override { return "Energy Lazer"; }
    private:
        bool animation_check = true;
        bool onetime = false;
        bool timer_on = false;

        float lazer_angle = Math::PI / 2;
    };
    State_Lazer state_lazer;

    class Player_Down : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Player Down!"; }
    };
    Player_Down state_player_down;

private:
    //other object
    DownTimer* hurt_timer;
    Player* player_;
    vec2 playerPosition = vec2({ 0, 0 });
    vec2 PrePlayerPosition = vec2({ 0, 0 });
    vec2 move_direction = { 0 ,0 };

    //blade
    vec2 screen_size = vec2(Engine::Instance()->Get_Window().x / 1.5f, Engine::Instance()->Get_Window().y / 1.5f);
    const vec2 BladeSize = vec2(400, 400);
    vec2 BladeColSize = BladeSize * 0.7f;
    vec2 Energy_kick_speed{ 100,0 };
    float Energy_kick_delay = 1.0f;
    int Energy_kick_count = 4;
    static constexpr float acceleration = 120;
    static constexpr float max_velocity = 240;
    static constexpr float drag = 700;
    float speed = 0;
    float hurt_time = 1.5f;
    int HP = 3;
    int EnhanceHP = 1;
    bool barrier = true;
    int barrier_HP = 4;
    int barrier_hited = 0;

    bool is_hurt = false;
    bool is_charged = false;
    bool is_created = false;
    bool is_teleported = false;
    bool is_animation_ended = true;
    bool particle_can_move = false;
    bool stop_move = false;
    bool is_collide = true;
    bool pattern_finished = false;
    bool sound_playing = false;

    //attack
    vec2 particle_vec2 = { 0,0 };
    float angle = 0;
    double attack_delay = 0;

    enum class Animations {
        Idle,
        Step,
        Running,
        Punch,
        Kick,
        Pattern_Kick1,
        Pattern_Kick2,
        Earthquake,
        Enerylazer1,
        Enerylazer2,
        Dash
    };

};