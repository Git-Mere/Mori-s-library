#include "Timer.h"

DownTimer::DownTimer(float time_remaining) {
    Set(time_remaining);
}

void DownTimer::Set(float time_remaining) {
    timer_max = time_remaining;
    isTick = false;
    Reset();
}

void DownTimer::ZeroSet()
{
    timer = 0;
}

void DownTimer::Update(float dt) {
    if (isTick) {
        isTick = false;
    }
    else {
        isTick = true;
    }
    timer -= dt;

    if (timer < 0) {
        timer = 0;
    }
}

void DownTimer::Reset() {
    timer = timer_max;
}

float DownTimer::Remaining() {
    return timer;
}

int DownTimer::RemainingInt() {
    return static_cast<int>(timer);
}

bool DownTimer::TickTock() {
    return isTick;
}

bool DownTimer::end()
{
    return timer <= 0;
}

///////////////////////////////// UpTimer (0,1,2) /////////////////////////////////
UpTimer::UpTimer(float time_remaining) {
    Set(time_remaining);
}

void UpTimer::Set(float time_remaining) {
    start_timer = time_remaining;
    Reset();
}

void UpTimer::Update(float dt) {

    timer += dt;
    if (timer > start_timer) {
        timer = start_timer;
    }
}

void UpTimer::Reset() {
    timer = 0.0f;
}

float UpTimer::Remaining() {
    return timer;
}

int UpTimer::RemainingInt() {
    return static_cast<int>(timer);
}

bool UpTimer::end()
{
    return timer >= start_timer;
}
