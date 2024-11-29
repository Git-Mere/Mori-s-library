#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include "Mori/States.h"
#include "../Mori/Player.h"

class Mori : public GameObject {
public:
    Mori(vec2 position, vec2 scale);
    //void Update([[maybe_unused]] float dt);
    //void Draw(const mat3& world_to_ndc);
    GameObjectTypes Type() override { return GameObjectTypes::Mori; }
    std::string TypeName() override { return "Mori"; }
    bool no_mori = false;
private:
};

class tuto_dummy;

class Mori_talk : public GameObject {
public:
    Mori_talk(Mori&, Player& player);
    void Update(float dt);
    //void Draw(const mat3& camera_matrix) override;
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object);
    bool tuto_animation_end();
    GameObjectTypes Type() override { return GameObjectTypes::Mori_talk; }
    std::string TypeName() override { return "Mori_talk"; }

private:
    //only tutorial//////////////////
    void first_tuto();
    void second_tuto();
    tuto_dummy* dummy = nullptr;
    ///////////////////////////////

    States what_state;
    bool collision_on = false;
    Mori* mori;
    Player* player;

    int starting_animation = Start;
    int limit = starting_animation;
    bool control_helper = true;

    enum Animations {
        Open = 1,
        Close,
        Start
    };

    class State_Open : public State {
    public:
        virtual void Enter( GameObject* object) override;
        virtual void Update( GameObject* object,  float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Open"; }
    private:
    };

    class State_Close : public State {
    public:
        virtual void Enter( GameObject* object) override;
        virtual void Update( GameObject* object,  float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Close"; }
    private:
    };

    class State_Say : public State {
    public:
        virtual void Enter( GameObject* object) override;
        virtual void Update( GameObject* object,  float dt) override;
        virtual void CheckExit(GameObject* object) override;
        std::string GetName() override { return "Say"; }
    private:
        bool next = false;
        bool lock = false;
        int tuto_mode = 0;
        int pre_ani = 0;
    };

    State_Open state_open;
    State_Close state_close;
    State_Say state_say;

    static constexpr int tutorial_count = 3;
};