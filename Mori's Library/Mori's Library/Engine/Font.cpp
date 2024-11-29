

#include "Font.h"
#include "Engine.h"
#include <iostream>

Font::Font(const std::filesystem::path& file_name){
    const unsigned int white = 0xFFFFFFFF;

    texture = Engine::GetTextureManager().FontLoad(file_name);


    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->image.GetHandle(), 0);
    glCheck(glNamedFramebufferReadBuffer(fbo, GL_COLOR_ATTACHMENT0));
    //
    // The top left pixel MUST be white (0xFFFFFFFF)
    unsigned int color = texture->GetPixel(ivec2(0, texture->GetSize().y - 1));
    if (color != white) {
        Engine::GetLogger().LogError("Font " + file_name.string() + " could not be loaded");
        return;
    }
    else {
        FindCharRects();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
}

void Font::FindCharRects() {
    int e_width = texture->GetSize().x;
    int height = texture->GetSize().y - 1;

    unsigned int check_color = texture->GetPixel({ 0, height });
    unsigned int next_color;

    int x = 1;
    int index = 0;
    while (check_color != 0xFF0000FF) {

        int width = 0;

        do {
            width++;
            next_color = texture->GetPixel({ x + width, height });
        } while (check_color == next_color);

        check_color = next_color;
        
        char_rects[index].point_1 = { x, 1 };
        char_rects[index++].point_2 = { x + width - 1, char_rects[index].Top() + height - 1};
        x += width;
        
    }
    char_rects[index].point_1 = { x, 1 };
    char_rects[index++].point_2 = { e_width, char_rects[index].Top() + height - 1 };
}

Math::irect& Font::GetCharRect(char c) {
    if (c >= ' ' && c <= 'z') {
        return char_rects[c - ' '];
    }
    else {
        Engine::GetLogger().LogError("Char '" + std::to_string(c) + "' not found");
        return char_rects[0];
    }
}

ivec2 Font::MeasureText(std::string text)
{
    ivec2 result;
    for (char c : text) {
        result.x += GetCharRect(c).Size().x;
        result.y = GetCharRect(c).Size().y;
    }
    return result;
}

Texture Font::PrintToTexture(std::string text) {
    ivec2 text_size = MeasureText(text);
    ivec2 entire_size = texture->GetSize();

    GLTexture glt;
    GLTexture::RGBA* data = texture->Get_GLtexture_data();
    GLTexture::RGBA* input_data = new GLTexture::RGBA[text_size.x * text_size.y];
    size_t num = text.size();
    std::vector<Math::irect> nemos{};
    for (int i = 0; i < num; i++) {
        nemos.push_back(GetCharRect(text[i]));        
    }
    int index = 0;
    for (int y = 1; y <= text_size.y; y++) {
        for (int j = 0; j < num; j++) {
            int left = nemos[j].Left();
            int size = left + nemos[j].Size().x;
            for (int x = left; x < size; x++) {
                input_data[index++] = data[x + (entire_size.x * y)];
            }
        }
    }

    glt.LoadFromMemory(text_size.x, text_size.y, input_data);

    Texture result(std::move(glt));
    delete input_data;
    return result;
}
