

#pragma once
#include <string>
#include "Texture.h"
#include "Vec2.h"
#include "Rect.h"


class Font{
public:
	Font(const std::filesystem::path& file_name);
	Texture PrintToTexture(std::string text);
private:
	void FindCharRects();
	Math::irect& GetCharRect(char c);
	ivec2 MeasureText(std::string text);

	Texture* texture;
	static constexpr int num_chars = 120;
	Math::irect char_rects[num_chars];
};
