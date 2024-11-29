
#include "Texture.h"
#include "Engine.h"

Texture::Texture(const std::filesystem::path& file_path, bool font) {
    if (font == false) {
        if (const bool loaded = image.LoadFromFileImage(file_path); !loaded) {
            throw std::runtime_error{ "Failed to load the duck.png" };
        }
    }
    else {
        if (const bool loaded = image.FontLoadFromFileImage(file_path); !loaded) {
            throw std::runtime_error{ "Failed to load the duck.png" };
        }
    }
    //image = doodle::Image{ file_path };
}

void Texture::Draw([[maybe_unused]] Math::TransformationMatrix display_matrix) {
    //doodle::push_settings();
    //doodle::apply_matrix(
    //    display_matrix[0][0],
    //    display_matrix[1][0],
    //    display_matrix[0][1],
    //    display_matrix[1][1],
    //    display_matrix[0][2],
    //    display_matrix[1][2]);
    //doodle::draw_image(image, 0, 0);
    //doodle::pop_settings();
}

void Texture::Draw([[maybe_unused]] Math::TransformationMatrix display_matrix, [[maybe_unused]] ivec2 texel_position, [[maybe_unused]] ivec2 frame_size) {
    //doodle::push_settings();
    //doodle::apply_matrix(display_matrix[0][0], display_matrix[1][0], display_matrix[0][1], display_matrix[1][1], display_matrix[0][2], display_matrix[1][2]);
    //doodle::draw_image(image, 0, 0, static_cast<double>(frame_size.x), static_cast<double>(frame_size.y), texel_position.x, texel_position.y);
    //doodle::pop_settings();
}


ivec2 Texture::GetSize() {
    return {
        image.GetWidth(), image.GetHeight()
    };
}

void Texture::UseForSlot(unsigned int texture_unit) const noexcept
{
    image.UseForSlot(texture_unit);
}

Texture::Texture(GLTexture&& img) {
    image = std::move(img);
}

unsigned int Texture::GetPixel(ivec2 texel) {
    int x = texel.x;
    int y = texel.y;

    unsigned char pixel[4];
    glCheck(glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel));

    uint32_t result = 0;
    
    result |= pixel[0] << 24;
    result |= pixel[1] << 16;
    result |= pixel[2] << 8;
    result |= pixel[3];

    return result;
}
