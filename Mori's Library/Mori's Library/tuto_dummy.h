#pragma once
#include "../Engine/GameObject.h"
#include "Mori/invincible.h"

class Player;
class FontObject;

enum Mode {
    Sandbag,
    Dodge,
    Parrying,
    End
};

class tuto_dummy : public GameObject {
public:

public:
    tuto_dummy(vec2 position, vec2 scale, Player& p_ptr, float rotation = 0 );
    void Draw(const mat3& world_to_ndc);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision([[maybe_unused]] GameObject* other_object);
    GameObjectTypes Type() override { return GameObjectTypes::Tuto_dummy; }
    std::string TypeName() override { return "tuto_dummy"; }
    void count();
    int Get_mode() { return mode; };
private:
    Invincible* invin;
    int mode = 0;
    vec2 start_position;
    bool mode_clear = false;
    FontObject* clear_count_font;



    enum class Animations {
        Idle,
        Throw
    };

    class State_Sandbag : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Sandbag"; }
        int clear_count = 0;
    private:
        static constexpr int clear_ = 5;
    };

    class State_Dodge : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* object) override;
        virtual void Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Dodge"; }
        int clear_count = 0;
    private:
        static constexpr int clear_ = 3;
    };

    class State_Parrying : public State {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Parrying"; }
        int clear_count = 0;
    private:
        static constexpr int clear_ = 3;
    };

    State_Sandbag state_sandbag;
    State_Dodge state_dodge;
    State_Parrying state_parrying;
    Player* player_;
    FontObject* explain1;
    static constexpr float hurt_time = 1.f;
    static constexpr float javeline_speed = 200.f;
    static constexpr float javeline_frequency = 4.f;


};

