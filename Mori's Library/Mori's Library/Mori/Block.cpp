#include "Block.h"
#include "../Engine/Sprite.h"

Block::Block(vec2 position, vec2 scale = vec2(30)) : GameObject(position), scale_size(scale) {
    AddGOComponent(new Sprite("assets/sprite/Book2/Block.spt", this, true));
    SetScale(scale_size);
}

void Block::SetBlockPosition(vec2 B_position) {
    SetPosition(B_position);
}