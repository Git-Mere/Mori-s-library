#pragma once
#include"../Engine/ComponentManager.h"
#include "../Engine/Engine.h"

class DownTimer : public Component {
public:
    DownTimer(float time_remaining);
    void Set(float time_remaining);
    void ZeroSet();
    void Update(float dt) override;
    void Reset();
    float Remaining();
    int RemainingInt();
    bool TickTock();
    bool end();
private:
    float timer;
    float timer_max;
    bool isTick;
};


class UpTimer : public Component {
public:
    UpTimer(float time_remaining);
    void Set(float time_remaining);
    void Update(float dt) override;
    void Reset();
    float Remaining();
    int RemainingInt();
    bool end();
private:
    float timer;
    float start_timer;
};