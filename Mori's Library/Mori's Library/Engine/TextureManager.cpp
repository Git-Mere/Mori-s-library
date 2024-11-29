

#include "TextureManager.h"
#include "Texture.h"
#include "Engine.h"

Texture* TextureManager::Load(const std::filesystem::path& file_path, bool font)
{
    if (font == false) {
        auto iter = textures.find(file_path);
        if (iter != textures.end()) {
            return iter->second;
        }
    }
    Texture* texture = new Texture(file_path);

    textures[file_path] = texture;
    Engine::GetLogger().LogEvent("Loading Texture: " + file_path.string());

    return texture;
}

Texture* TextureManager::FontLoad(const std::filesystem::path& file_path)
{
    auto iter = textures.find(file_path);
    if (iter != textures.end()) {
        return iter->second;
    }

    Texture* texture = new Texture(file_path, true);

    //textures[file_path] = texture;
    //Engine::GetLogger().LogEvent("Loading Texture: " + file_path.string());

    return texture;
}

void TextureManager::Unload()
{
    Engine::GetLogger().LogEvent("Clearing Textures");

    for (auto& pair : textures) {
        delete pair.second;
    }
    textures.clear();
}