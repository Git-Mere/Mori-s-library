#pragma once
#include "../Engine/GameObject.h"

class GameOver : public GameObject {
public:
    GameOver(vec2 position);
    GameObjectTypes Type() override { return GameObjectTypes::GameOver; }
    std::string TypeName() override { return "Game Over"; }

    void RePosition(vec2 reposition) {
        SetPosition(reposition);
    }
private:
    vec2 scale = vec2(700, 400);
};

class PressAnyKey : public GameObject {
public:
    PressAnyKey(vec2 position);
    GameObjectTypes Type() override { return GameObjectTypes::PressAnyKey; }
    std::string TypeName() override { return "Press Any Key"; }

    void RePosition(vec2 reposition) {
        SetPosition(reposition);
    }
private:
    vec2 scale = vec2(451, 50);
};

class Screen_Effect : public GameObject {
public:
    Screen_Effect(vec2 position);
    GameObjectTypes Type() override { return GameObjectTypes::Screen_Effect; }
    std::string TypeName() override { return "Dark Screen"; }

    void RePosition(vec2 reposition) {
        SetPosition(reposition);
    }
private:
    vec2 scale = vec2(1580, 1020);
};

class GameClear : public GameObject {
public:
    GameClear(vec2 position);
    GameObjectTypes Type() override { return GameObjectTypes::GameClear; }
    std::string TypeName() override { return "Game Clear"; }

    void RePosition(vec2 reposition) {
        SetPosition(reposition);
    }
private:
    vec2 scale = vec2(700, 400);
};