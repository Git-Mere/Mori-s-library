#include "FontObject.h"
#include "../Engine/Sprite.h"

FontObject::FontObject(vec2 position, std::string text) : GameObject(position) {
    AddGOComponent(new Sprite("assets/sprite/FontObject.spt", this, true));
    GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(text));
    SetScale(Engine::Instance()->Get_Window() / 10);
}

void FontObject::change_text(std::string str)
{
    GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(str));
}

void FontObject::SetFontSize(vec2 font_size) {
    SetScale(font_size);
}

void FontObject::SetFontPosition(vec2 pos)
{
    SetPosition(pos);
}

FontObject::FontObject(vec2 position, vec2 scale = vec2(30)) : GameObject(position), scale_size(scale) {
    AddGOComponent(new Sprite("assets/sprite/FontObject.spt", this, true));
    SetScale(scale_size);
}