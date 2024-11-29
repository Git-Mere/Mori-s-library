#include "Sprite.h"
#include "Engine.h"
#include "Collision.h"
#include <iostream>

std::span<const float, 4> to_span(const vec4& v)
{
    return std::span<const float, 4>(&v.elements[0], 4);
}

Sprite::Sprite(const std::filesystem::path& sprite_file, bool font) : object(nullptr), texture(nullptr)
{
    Load(sprite_file, font);
}

Sprite::Sprite(const std::filesystem::path& sprite_file, GameObject* obj, bool font) : object(obj), texture(nullptr) {
    Load(sprite_file, font);
}

Sprite::~Sprite()
{
    animations.clear();

    for (int i = 0; i < animations.size(); i++) {
        delete animations[i];
    }
}

void Sprite::Update(float dt)
{
    animations[current_animation]->Update(dt);
}

void Sprite::Load(const std::filesystem::path& sprite_file, bool font) {
    if (sprite_file.extension() != ".spt") {
        throw std::runtime_error(sprite_file.generic_string() + " is not a .spt file");
    }
    std::ifstream in_file(sprite_file);

    if (in_file.is_open() == false) {
        throw std::runtime_error("Failed to load " + sprite_file.generic_string());
    }
    animations.clear();

    std::string text;
    in_file >> text;

    while (in_file.eof() == false) {
        if (text == "FrameSize") {
            in_file >> frame_size.x;
            in_file >> frame_size.y;
        }
        else if (text == "Image") {
            std::string path{};
            in_file >> path;
            texture = Engine::GetTextureManager().Load(path, font);
            frame_size = Engine::GetTextureManager().Load(path)->GetSize();
        }
        else if (text == "NumFrames") {
            int frame_count_x;
            in_file >> frame_count_x;
            Numframe.x = frame_count_x;
        }
        else if (text == "YNumFrames") {
            int frame_count_y;
            in_file >> frame_count_y;
            Numframe.y = frame_count_y;
        }
        else if (text == "HotSpot") {
            float hotspot_x, hotspot_y;
            in_file >> hotspot_x;
            in_file >> hotspot_y;
            hotspots.push_back({ hotspot_x, hotspot_y });
        }
        else if (text == "Anim") {
            std::string anim;
            in_file >> anim;
            animations.push_back(new Animation{ anim });
        }
        else if (text == "Collision") {
            std::string what_col;
            in_file >> what_col;
            if (object == nullptr) {
                Engine::GetLogger().LogError("Cannot add collision to a null object");
            }
            else {
                if (what_col == "Rect") {
                    object->AddGOComponent(new RectCollision(object));
                }
                else if (what_col == "Circle") {
                    object->AddGOComponent(new CircleCollision(object));
                }
                else if (what_col == "Line") {
                    float x, y, x2, y2;
                    std::vector<vec2> input;
                    in_file >> x >> y >> x2 >> y2;
                    input.push_back(vec2(x, y));
                    input.push_back(vec2(x2, y2));
                    object->AddGOComponent(new LineCollision(object, input));
                }
                else if (what_col == "Point") {
                    object->AddGOComponent(new PointCollision(object));
                }
                else if (what_col == "Poly") {
                    std::vector<vec2> input;
                    float x, y;
                    while (in_file >> x >> y) {
                        
                        vec2 tmp(x, y);
                        input.push_back(tmp);

                        char newline;
                        in_file.get(newline);
                        if (newline == '\n') {
                            break;
                        }
                    }
                    object->AddGOComponent(new PolyCollision(object, input));
                }

            }
        }
        else {
            Engine::GetLogger().LogError("Unknown command: " + text);
        }
        in_file >> text;
    }

    if (animations.empty() == true) {
        animations.push_back(new Animation{});
    }
    if (hotspots.empty() == true) {
        hotspots.push_back(vec2(0));
    }

    PlayAnimation(0);
}


void Sprite::Draw() {
    Engine::ShaderName shader = object->GetShaderName();

    Engine::Instance()->Get_Shader(shader).Use();
    omit_tex_coordinate();
    if (texture != nullptr) {
        glCheck(texture->UseForSlot(0));
    }
    else {
        omit_color();
    }
    Engine::Instance()->Get_Model(Engine::ModelName::Rect).Use();

    glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uTex2d", 0));
    glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("tex_coord_scale", texture_scale));
    glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uModelToNDC", to_span(object->final_matrix)));
    glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Rect)));

}

void Sprite::No_Gameobject_Draw(const mat3& matrix)
{
    Engine::ShaderName shader = Engine::ShaderName::basic;

    Engine::Instance()->Get_Shader(shader).Use();
    omit_tex_coordinate();
    if (texture != nullptr) {
        glCheck(texture->UseForSlot(0));
    }
    else {
        omit_color();
    }
    Engine::Instance()->Get_Model(Engine::ModelName::Rect).Use();

    glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uTex2d", 0));
    glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uModelToNDC", to_span(matrix)));
    glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Rect)));
}


vec2 Sprite::GetHotSpot(int idx)
{
    if (idx >= hotspots.size() || idx < 0) {
        Engine::GetLogger().LogError("Invalid index");
        return { 0,0 };
    }
    return hotspots[idx];
}

ivec2 Sprite::GetFrameSize()
{
    return frame_size;
}

void Sprite::PlayAnimation(int animation)
{
    if (animation < 0 || animation >= animations.size()) {
        Engine::GetLogger().LogError("the animation doesnt exist.");
    }
    else {
        if (animation == prev_animation) {
            return;
        }
        current_animation = animation;
        prev_animation = animation;
        animations[current_animation]->Reset();
    }
}

void Sprite::ResetAnimation()
{
    animations[current_animation]->Reset();
}

void Sprite::Setframe(int index, int index2)
{
    animations[index]->Set_current(index2);
}


bool Sprite::AnimationEnded()
{
    if (animations[current_animation]->Ended()) {
        return true;
    }
    return false;
}

void Sprite::Set_texture_sclae(float value)
{
    texture_scale = value;
}

void Sprite::omit_tex_coordinate()
{
    std::vector<vec2>  tex_pos_vtx = Engine::Instance()->Get_texture_ndc();
    ivec2 frame = get_frame();//
    float how = 1.0f / Numframe.x;
    float how2 = 1.0f / Numframe.y;
    tex_pos_vtx[0].x = how * frame.x - how;
    tex_pos_vtx[1].x = how * frame.x;
    tex_pos_vtx[2].x = how * frame.x;
    tex_pos_vtx[3].x = how * frame.x - how;

    tex_pos_vtx[0].y = how2 * frame.y - how2;
    tex_pos_vtx[1].y = how2 * frame.y - how2;
    tex_pos_vtx[2].y = how2 * frame.y;
    tex_pos_vtx[3].y = how2 * frame.y;

    Engine::Instance()->Get_Model(Engine::ModelName::Rect).GetVertexBuffers().at(1).SetData(std::span{ tex_pos_vtx });
}

void Sprite::stretch_texture(vec2 size)
{
    std::vector<vec2>  tex_pos_vtx = Engine::Instance()->Get_texture_ndc();
    //float frame = get_frame();
    //float how = 1.0f / Numframe;
    //tex_pos_vtx[0] *= tex_pos_vtx[0] * size;
    tex_pos_vtx[1].x *= size.x;
    tex_pos_vtx[2].x *= size.x;
    //tex_pos_vtx[3].x = how * frame - how;

    Engine::Instance()->Get_Model(Engine::ModelName::Rect).GetVertexBuffers().at(1).SetData(std::span{ tex_pos_vtx });
}

void Sprite::omit_color()
{
    vec4  color = object->Getcolor();
    glCheck(Engine::Instance()->Get_Shader(object->GetShaderName()).SendUniform("uFillColor", to_span(color)));
}

void Sprite::Set_texture(Texture new_texture)
{
    *texture = std::move(new_texture);
}