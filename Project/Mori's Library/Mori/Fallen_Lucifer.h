#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include <Random.h>
#include "Timer.h"
#include "player.h"

class Fallen_Lucifer : public GameObject {
public:
    Fallen_Lucifer(vec2 position, Player& player);
    GameObjectTypes Type() override { return GameObjectTypes::Fallen_Lucifer; }
    std::string TypeName() override { return "Fallen Lucifer"; }

    void Update(float dt)override;
    void Draw(const mat3& world_to_ndc);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object) override;
    bool Fallen_LuciferDead();
    const vec2 Return_coordinate();
    vec2 particle_vec[16];
    vec2 particle_vec2 = { 0, 0 };

    void update_velocity(float dt);
    float ReturnAngle() { return angle; }
    float ReturnParticleMove() { return particle_can_move; }
    float ReturnIsAnimationEnded() { return is_animation_ended; }

    //Default
    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    State_Idle state_idle;

    //뚜벅뚜벅
    class State_Running : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Running"; }
    private:
        int pattern_number = 0;
    };
    State_Running state_running;

    //attack hell
    class State_Hell : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Hell"; }
    private:
        int Pit_count = 0;
        int Pit_count_max = 60;
        float Pit_timer = 0.0f;
        float Pit_timer_max = 2.5f;

        int Pillar_count = 0;
        int Pillar_count_max = 8;
        float pillar_timer = 0.0f;
        float pillar_timer_max = 3.5f;
    };
    State_Hell state_hell;

    //패링하세요 팅 티딩 티디딩
    class Fallen_Javelin : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Fallen Javelin"; }

    private:
        float pre_angle = 0;
        float orbitX = 0;
        float orbitY = 0;
    };
    Fallen_Javelin javelin;

    //기모으고
    class Charging_Dark : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Charging Dark"; }
    };
    Charging_Dark charging_dark;

    //원형 짤패턴
    class Dark_Sword : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Dark Sword"; }
    };
    Dark_Sword dark_sword;

    //레이저 빙글빙글
    class Judgment : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Judgment"; }
    };
    Judgment judgment;

    //순간이동 후 티디딩
    class Teleport : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Teleport"; }
    };
    Teleport teleport;

    //Player Down!
    class Player_Down : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Player Down!"; }
    };
    Player_Down state_player_down;

private:
    vec2 teleport_place();
    //other object
    DownTimer* hurt_timer;
    Player* player_;
    vec2 playerPosition = vec2({ 0, 0 });

    //blade
    const vec2 scale = vec2(300, 300);
    vec2 ColSize = vec2(70, 100);

    int light_pos_value = 0;
    int num = 0;
    int create_count = 0;
    int random_num = 0;
    float speed = 0;
    float pattern_count = 0;
    float Distance = 750;
    float Distance2 = 350;
    float angle = 0;
    float root2 = 1.4f;
    float hurt_time = 1.0f;
    int dark_sword_count = 0;
    float divine_dark_angle[8];
    bool sound_one = false;

    vec2 current_pos = { 0,0 };
    vec2 light_position[16];
    std::vector<vec2> appear_position;
    vec2 move_direction = { 0 ,0 };

    static constexpr float acceleration = 50;
    static constexpr float max_velocity = 100;
    static constexpr float drag = 700;

    bool is_hurt = false;
    bool is_charged = false;
    bool is_created = false;
    bool is_teleported = false;
    bool is_animation_ended = true;
    bool particle_can_move = false;
    bool wing_is_created = false;
    bool stop_move = false;
    bool is_collision = true;
    bool pattern_finished = false;
    bool is_dead = false;

    double attack_delay = 0;
    double charge_delay = 0;

    enum class Animations {
        Idle,
        teleport
    };
};