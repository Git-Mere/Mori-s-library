/*!************************************************************************
\file GLTexture.h
\author Seungheon Jeon, Rudy Castan
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par CS200 Tutorial 5 - Texture MappingVirtual programming lab
\date 10-05-2023
\brief
This header file make GLTexture class and declare function of this
**************************************************************************/

#include <array>
#include <filesystem>
#include <stb_image.h>

#include "GL/glew.h"
#include "GLHandle.h"
#include "glCheck.h"

#pragma once

class [[nodiscard]] GLTexture
{
public:
    GLTexture() = default;
    ~GLTexture() noexcept;

    GLTexture(const GLTexture& other) = delete;

    GLTexture(GLTexture&& other) noexcept;
    GLTexture& operator=(const GLTexture& other) = delete;

    GLTexture& operator=(GLTexture&& other) noexcept;


    [[nodiscard]] bool LoadFromFileImage(const std::filesystem::path& image_filepath, bool flip_vertical = true) noexcept;
    [[nodiscard]] bool FontLoadFromFileImage(const std::filesystem::path& image_filepath, bool flip_vertical = true) noexcept;
    using RGBA = unsigned int;
    static_assert(sizeof(RGBA) == 4, "RGBA should be the same as 4 bytes, so we can easily treat it as an RGBA int.");


     bool LoadFromMemory(int image_width, int image_height, const RGBA* colors) noexcept;


    void UseForSlot(unsigned int texture_unit) const noexcept;

    [[nodiscard]] unsigned int GetHandle() const noexcept
    {
        return texture_handle;
    }
    [[nodiscard]] int GetWidth() const noexcept
    {
        return width;
    }
    [[nodiscard]] int GetHeight() const noexcept
    {
        return height;
    }
    [[nodiscard]] RGBA* GetData() const noexcept
    {
        return data;
    }
    enum Filtering : GLint
    {
        NearestPixel = GL_NEAREST,
        Linear       = GL_LINEAR
    };

    void                    SetFiltering(Filtering how_to_filter) noexcept;
    [[nodiscard]] Filtering GetFiltering() const noexcept
    {
        return filtering;
    }
    enum Coordinate
    {
        S,
        T,
        Both
    };

    enum Wrapping : GLint
    {
        Repeat            = GL_REPEAT,
        ClampToEdge       = GL_CLAMP_TO_EDGE,
        ClampToBorder     = GL_CLAMP_TO_BORDER,
        MirroredRepeat    = GL_MIRRORED_REPEAT,
        MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
    };

    void SetWrapping(Wrapping how_to_wrap, Coordinate coord = Coordinate::Both) noexcept;

    [[nodiscard]] std::array<Wrapping, 2> GetWrapping() const noexcept
    {
        return wrapping;
    }

private:
    void delete_texture() noexcept;

private:
    GLHandle                texture_handle = 0;
    int                     width          = 0;
    int                     height         = 0;
    Filtering               filtering      = Filtering::NearestPixel;
    std::array<Wrapping, 2> wrapping       = {Wrapping::Repeat, Wrapping::Repeat};
    RGBA*          data;
};
