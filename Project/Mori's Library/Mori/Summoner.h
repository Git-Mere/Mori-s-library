#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/GameState.h"
#include "../Engine/Particle.h"
#include "Player.h"
#include "Timer.h"

class Summoner : public GameObject {
public:
	Summoner(vec2 position, Player& player);
	GameObjectTypes Type() override { return GameObjectTypes::Summoner; }
	std::string TypeName() override { return "summoner"; }

	void Update(float dt)override;
	void Draw(const mat3& world_to_ndc);
	bool CanCollideWith(GameObjectTypes other_object_type);
	void ResolveCollision(GameObject* other_object) override;
	void update_velocity(float dt);

	const vec2 Return_coordinate();

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

private:
    DownTimer* hurt_timer;
    Player* player_;
    vec2 playerPosition = vec2({ 0, 0 });
    vec2 move_direction = { 0 ,0 };

    const vec2 scale = vec2(50, 50);
    vec2 ColSize = vec2(50, 50);

    float hurt_time = 1.0f;
    float speed = 0;

    bool is_hurt = false;
    bool stop_move = false;

    bool is_pattern_finished = false;
    double attack_delay = 0;

    bool is_boom = false;


    static constexpr float acceleration = 300;
    static constexpr float max_velocity = 500;
    static constexpr float drag = 700;

    enum class Animations {
        Idle,
        explode
    };
};