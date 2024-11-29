#pragma once
#include <string>
#include "Vec2.h"
#include "Texture.h"
#include "Matrix.h"
#include "Animation.h"
#include "Component.h"
#include "GameObject.h"
#include <mat3.h>

class GameObject;
class Sprite : public Component {
public:
    Sprite() {};
    Sprite(const std::filesystem::path& sprite_file, bool font = false);
    Sprite(const std::filesystem::path& sprite_file, GameObject* obj, bool font = false);

    ~Sprite();
    void Update(float dt) override;
    void Load(const std::filesystem::path& sprite_file, bool font = false);
    void Draw();
    void No_Gameobject_Draw(const mat3& matrix);
    vec2 GetHotSpot(int index);
    ivec2 GetFrameSize();

    void PlayAnimation(int animation);
    void ResetAnimation();
    void Setframe(int index, int index2);
    bool AnimationEnded();
    int CurrentAnimation() { return current_animation; }
    ivec2 get_frame() { return animations[current_animation]->CurrentFrame(); }
    int get_animation_num() { return static_cast<int>(animations.size()); }
    void Set_texture_sclae(float value);

    void omit_tex_coordinate();

    void stretch_texture(vec2 size);

    void omit_color();

    void Set_texture(Texture new_texture);
private:
    Texture* texture;
    std::vector<vec2> hotspots;
    ivec2 Numframe = ivec2{1,1};

    int current_animation = 0;
    int prev_animation = 0;
    ivec2 frame_size;
    std::vector<Animation*> animations;
    GameObject* object;
    float texture_scale = 1.f;
};