#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Rect.h"
#include "../Engine/Constant.h"
#include "Timer.h"
#include "Player.h"
#include "captain.h"

class Captain_attack : public GameObject {
public:
    Captain_attack(vec2 position, Player& player, Captain& captain);
    void Update(float dt);
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision([[maybe_unused]] GameObject* other_object);
    GameObjectTypes Type() override { return GameObjectTypes::Captain_attack; }
    std::string TypeName() override { return "Captain_attack"; }
    void Load(const std::filesystem::path& pattern_file);
    bool GetFinish() {
        return finish;
    }
    int GetIndex() {
        return index;
    }
    vec2 Get_font_size();
private:
    void re_position();
    bool success_parrying = false;
    bool finish = false;

    vec2 right_left_size{ 150, 480 };
    vec2 right_left_size_minus{ -150, 480 };
    Player* play_ptr;
    Captain* captain_ptr;
    enum type {
        Wait,
        Left,
        Right,
        Middle,
        End
    };

    std::vector<std::pair<type, float>> data;
    std::vector<vec2> font_size_vec;
    int index = 0;

    class State_Wait : public State {
    public:
        virtual void Enter(GameObject* obj) override;
        virtual void Update(GameObject* obj, float dt) override;
        virtual void CheckExit(GameObject* obj) override;
        std::string GetName() override { return "Wait"; }
    private:
        DownTimer timer{0};
    };

    class State_LeftRight : public State {
    public:
        virtual void Enter(GameObject* obj) override;
        virtual void Update(GameObject* obj, float dt) override;
        virtual void CheckExit(GameObject* obj) override;
        std::string GetName() override { return "LeftRight"; }
    private:
        float speed;
        type _type;
        DownTimer timer{ 0 };
        float parrying_delay = 0.3f;
    };

    class State_Middle : public State {
    public:
        virtual void Enter([[maybe_unused]] GameObject* obj) override;
        virtual void Update([[maybe_unused]] GameObject* obj, [[maybe_unused]] float dt) override;
        virtual void CheckExit(GameObject* obj) override;
        std::string GetName() override { return "Middle"; }
    private:
        float speed;
        DownTimer timer{ 0 };
        static constexpr float parrying_delay = 0.3f;
    };

    State_Wait state_wait;
    State_LeftRight state_leftright;
    State_Middle state_middle;
};

