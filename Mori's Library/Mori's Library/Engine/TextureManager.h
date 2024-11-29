

#pragma once
#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>

class Texture;

class TextureManager {
public:
    Texture* Load(const std::filesystem::path& file_path, bool font = false);
    Texture* FontLoad(const std::filesystem::path& file_path);
    void Unload();

private:
    std::map<std::filesystem::path, Texture*> textures;
};