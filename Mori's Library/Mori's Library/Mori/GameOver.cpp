#include "GameOver.h"

GameOver::GameOver(vec2 position) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/GameOver/GameOver.spt", this));
    SetScale(scale);
}

PressAnyKey::PressAnyKey(vec2 position) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/GameOver/PressAnyKey.spt", this));
    SetScale(scale);
}

Screen_Effect::Screen_Effect(vec2 position) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/Portalbook/portalbook.spt", this));
    SetScale(scale);
    SetShaderName(Engine::Dark);
}

GameClear::GameClear(vec2 position) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/GameOver/GameClear.spt", this));
    SetScale(scale);
}