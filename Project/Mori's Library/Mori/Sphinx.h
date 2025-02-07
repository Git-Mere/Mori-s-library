#pragma once
#include "../Engine/GameObject.h"
#include "Timer.h"
#include "Player.h"

class Sphinx : public GameObject {
public:
    Sphinx(vec2 position, vec2 scale, Player& player);
    void Update([[maybe_unused]] float dt);
    void Draw(const mat3& world_to_ndc);
    GameObjectTypes Type() override { return GameObjectTypes::Sphinx; }
    std::string TypeName() override { return "Sphinx"; }
    void ResolveCollision(GameObject* other_object) override;

    int SphinxHelth();
    void SetSphinxPosition(vec2 reposition);
    void Rest();
    void Active(vec2 reposition);
    int MaxLife();


private:
    //player
    Player* player_;
    vec2 playerPosition = vec2({ 0, 0 });

    //movement
    vec2 move_direction = vec2{ 0,0 };

    //life
    DownTimer* hurt_timer;
    float hurt_time = 1.0f;
    bool is_hurt = false;
    int life = 15;

    //patern
    int pattern_count = 0;
    bool is_active = false;
    bool ontime = false;
    float attack_delay = 0.0f;
    bool hide = false;
    float time = 0;
    float max_time = 1.2f;
};