#include "sample_logo.h"
#include "../Engine/Sprite.h"


Logo::Logo(vec2 position) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/Digipen.spt", this));
    SetScale(vec2(400, 400));
    SetShaderName(Engine::logo);
}