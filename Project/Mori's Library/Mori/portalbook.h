#pragma once
#include "../Engine/GameObject.h"
#include "../Mori/Timer.h"

#include "portalbook_image.h"

class portalbook : public GameObject {
public:
    portalbook(vec2 position, int what_state, std::string book_name, Engine::ShaderName shader_name = Engine::bright);

    GameObjectTypes Type() override { return GameObjectTypes::Portalbook; }
    std::string TypeName() override { return "portalbook"; }
    void Update(float dt) override;
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision(GameObject* other_object);
    void ChangePosition(vec2 position);
    void go_state();

private:
    int to_state;
    bool one_coliision = true;
    static constexpr float jump_waiting = 2.f;
    float dist = 0;
    float open_dist = 150;
    std::string file_name;
    Engine::ShaderName recation_shader_name;
    portalbook_image* image_ptr = nullptr;
};