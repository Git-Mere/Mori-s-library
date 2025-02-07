

#pragma once
#include "../Engine/Texture.h"
#include "../Engine/CameraCom.h"
#include "../Engine/Component.h"
#include "../Engine/Sprite.h"

class Background : public Component {
public:
    void Add(const std::filesystem::path& texturePath, float speed, vec2 scale, vec2 position = vec2(0,0));
    void Update(float dt) override;
    void Unload();
    void Draw(const CameraComponent& camera);
    ivec2 GetSize();
private:
    struct ParallaxLayer {
        Sprite* texture;
        float speed = 1.f;
        vec2 scale{};
        vec2 position{};
    };

    std::vector<ParallaxLayer> backgrounds;
};
