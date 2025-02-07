#pragma once
#include "../Engine/GameState.h"

class Rolling_Credit : public GameState {
public:
    void Load() override;
    void Update([[maybe_unused]] float dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override { return "Rolling_Credit"; }
private:
  
    
};

class Credits : public GameObject {
public:
    Credits(const std::string& spritePath, vec2 position);
    void Update([[maybe_unused]] float dt);
    //void Draw(const mat3& world_to_ndc);
    GameObjectTypes Type() override { return GameObjectTypes::Just_Image; }
    std::string TypeName() override { return "Credits"; }
private:
    float rolling_speed = 100.0f;
};