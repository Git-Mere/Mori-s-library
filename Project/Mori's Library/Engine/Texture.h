
#pragma once
#include "Vec2.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "GLTexture.h"


class Texture {
public:
	friend class Font;
	friend class TextureManager;

	void Draw([[maybe_unused]] Math::TransformationMatrix display_matrix);
	void Draw([[maybe_unused]] Math::TransformationMatrix display_matrix, [[maybe_unused]] ivec2 texel_position, [[maybe_unused]] ivec2 frame_size);
	ivec2 GetSize();
	Texture() {}
	void UseForSlot(unsigned int texture_unit) const noexcept;
	GLTexture::RGBA* Get_GLtexture_data() {
		return image.GetData();
	}
private:
	GLTexture image;
	Texture(GLTexture&& img);
	Texture(const std::filesystem::path& file_path, bool font = false);
	unsigned int GetPixel(ivec2 texel);

};
