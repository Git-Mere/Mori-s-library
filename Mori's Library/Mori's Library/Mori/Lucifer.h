#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include <Random.h>
#include "Timer.h"
#include "player.h"

class Lucifer : public GameObject {
public:
    Lucifer(vec2 position, Player& player);
    GameObjectTypes Type() override { return GameObjectTypes::Lucifer; }
    std::string TypeName() override { return "Lucifer"; }

    void Update(float dt)override;
    void Draw(const mat3& world_to_ndc);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object) override;
    bool LuciferDead();
    const vec2 Return_coordinate();
    vec2 particle_vec[8];
    vec2 particle_vec2 = { 0, 0 };

    void update_velocity(float dt);
    float ReturnAngle() { return angle; }
    float ReturnParticleMove() { return particle_can_move; }
    float ReturnIsAnimationEnded() { return is_animation_ended; }



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

    vec2 current_pos = { 0,0 };
    vec2 light_position[8];
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
    };
    State_Running state_running;

    //기모으고
    class Charging_light : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Charging_light"; }
    };
    Charging_light charging_light;

    //레이저 빙글빙글
    class Judgment : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Judgment"; }
    };
    Judgment judgment;

    //원형 짤패턴
    class Holy_Sword : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Holy_Sword"; }
    };
    Holy_Sword holy_sword;

    //패링하세요 팅 티딩 티디딩
    class Javelin : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Javelin"; }
    };
    Javelin javelin;

    //순간이동 후 티디딩
    class Teleport : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Teleport"; }
    };
    Teleport teleport;

    //player Down
    class Player_Down : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* slime) override;
        virtual void Update([[maybe_unused]] GameObject* slime, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* slime) override;
        std::string GetName() override { return "Player Down!"; }
    };
    Player_Down state_player_down;

};