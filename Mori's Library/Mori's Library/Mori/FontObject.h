#pragma once
#include "../Engine/GameObject.h"

class FontObject : public GameObject {
public:
    FontObject(vec2 position, vec2 scale);
    FontObject(vec2 position, std::string text);
    void change_text(std::string str);
    GameObjectTypes Type() override { return GameObjectTypes::Just_Image; }
    std::string TypeName() override { return "Font Object"; }
    void SetFontSize(vec2 font_size);
    void SetFontPosition(vec2 pos);
private:
    vec2 scale_size;
};

