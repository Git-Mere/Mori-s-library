#pragma once
#include "Player.h"
#include "../UIManager.h"
#include "../Engine/Sprite.h"


enum Ani {
    exists,
    dis_effect
};

class Life : public Component, public UI {
public:
    Life(int life_number, bool is_player = false);
    ~Life();
    void Update(float dt) override;
    void Draw(const mat3& camera_matrix) override;
    bool Dead();
    void Sub(int how);
    void Add(int how);
    int GetLifeNumber();
    UITypes Type() { return UITypes::Life; };
private:
    std::vector<Sprite*> hearts;
    int life_number;
    bool for_player = false;
};