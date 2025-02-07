#pragma once
#include "../Engine/GameObject.h"
#include "invincible.h"
#include "States.h"
#include "Soundeffect.h"

enum class Animations {
    Look_Foward,
    Look_Backward,
    Look_Left,
    Look_Right,
    Move_Down,
    Move_Up,
    Move_Left,
    Move_Right,
    Dash_Down,
    Dash_Up,
    Dash_Left,
    Dash_Right,
    Parrying_Down,
    Parrying_Up,
    Parrying_Left,
    Parrying_Right
};

class SkillManager;

class Player : public GameObject {
    friend class Skill;
public:
    Player(vec2 position, vec2 scale, float rotation = 0);
    void Update(float dt);
    void Draw(const mat3& world_to_ndc);
    GameObjectTypes Type() override { return GameObjectTypes::Player; }
    std::string TypeName() override { return "Player"; }
    
    void update_velocity(float dt);
    void update_move_direction();
    void determine_animation();
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object) override;
    void hurt(GameObject* other_object, int life_down, bool other_collision_on = false, float time = hurt_time);
    void shove(GameObject* other_object);

    vec2& Get_move_dir();
    bool Get_parrying_mode();
    Skill* Get_Skill(int i);
    bool Get_invin();
    bool Get_stop_skill();
    States Get_what_state();
    int Get_Life();

    void Set_invin_timer(float time);
    void Set_shock_timer(float time);
    void Set_special_ani(bool value);
    void change_player_scale(vec2 scale);
    
    int parrying_count = 0;
    bool can_attack = false;

    vec2 particle_vec2 = { 0, 0 };
    void combo_damage()
    {
        damage += 1;
    }
    int get_damage()
    {
        return damage;
    }
    void reset_damage()
    {
        damage = 1;
    }
    void reset_combo_time()
    {
        combo_time = 0;
    }
    void set_combo_time(float n)
    {
        combo_time += n;
    }
    float get_combo_time()
    {
        return combo_time;
    }
    void Set_stop_move(bool stopmove) {
        stop_move = stopmove;
    }

private:
    void check_state();
    void cant_beyond_screen();
    void cant_beyond_wall(GameObject* other_object);

    SkillManager* skillmanager;
    Invincible* invin;
    DownTimer* shocktimer;

    vec2 move_direction = { 0 ,0 };
    vec2 player_scale = vec2(200.f);
    bool stop_move = true;
    bool stop_skill = false;
    bool yes_gravity = false;
    States what_state;
    int last_animation = 0;
    bool special_ani = false;
    bool sound_on = false;
    class State_Idle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* player) override;
        virtual void Update([[maybe_unused]] GameObject* player, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* player) override;
        std::string GetName() override { return "Idle"; }
    };

    class State_Running : public State {
    public:
        virtual void Enter(GameObject* player) override;
        virtual void Update(GameObject* player, float dt) override;
        virtual void CheckExit(GameObject* player) override;
        std::string GetName() override { return "Running"; }
    };

    State_Idle state_idle;
    State_Running state_running;

    static constexpr float x_acceleration = 400;
    static constexpr float y_acceleration = 400;
    static constexpr float max_velocity = 300;
    static constexpr float x_drag = 700;
    static constexpr float y_drag = 700;
    static constexpr float dash_speed = 500.f;
    static constexpr float hurt_time = 1.0f;
    static constexpr float hurt_speed = -300.0f;

    
    int damage = 1;
    float combo_time = 0;
};