#include "ScreenEffect.h"
#include "Engine/Engine.h"

effect::Shake::Shake(CameraComponent& camera, float time, float strong, int dir)
    : camera(&camera), timer(time), how_strong(strong), direction(dir)
{
}

void effect::Shake::Update(float dt)
{
    timer.Update(dt);
    if (timer.Remaining()) {
        shake();
    }
}

void effect::Shake::shake()
{
    vec2 strong = vec2(how_strong, 0);
    if (direction == 2) {
        strong = vec2(0, how_strong);
    }
    vec2 now = camera->GetPosition();
    if (shake_count == shake_limit) {
        if (dir) {
            camera->SetPosition(now + strong);
            dir = !dir;
        }
        else {
            camera->SetPosition(now - strong);
            dir = !dir;
        }
        shake_count = 0;
    }
    shake_count++;
}

void effect::Shake::Set_info(float time, int limit, float strong, int compass)
{
    timer.Set(time);
    how_strong = strong;
    direction = compass;
    shake_limit = limit;
}


effect::help_box::help_box(Engine::ShaderName name) : shader(name), timer(0)
{
    vec2 pos = vec2(0,0);
    vec2 ui_size{ Engine::Instance()->Get_Window() * 5 };

    const mat3 T = mat3::build_translation(pos);
    const mat3 H = mat3::build_scale(ui_size);
    matrix = T * H;

    Set_basic_color();

}

void effect::help_box::Update(float dt)
{
    timer.Update(dt);
}

void effect::help_box::Draw(const mat3& world_to_ndc)
{
    if (timer.end() == false) {
        mat3 final_matrix = world_to_ndc * matrix;
        Engine::Instance()->Get_Shader(shader).Use();
        Engine::Instance()->Get_Model(Engine::ModelName::Rect).Use();

        glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uModelToNDC", std::span<const float, 3 * 3>(&final_matrix.elements[0][0], 9)));
        glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uTime", timer.Remaining()));
        glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uDepart", depart));
        glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uResolution", std::span<const float, 2>(&Engine::Instance()->Get_Window().elements[0], 2)));
        glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uHelper", std::span<const float, 4>(&any_helper.elements[0], 4)));
        glCheck(Engine::Instance()->Get_Shader(shader).SendUniform("uFillColor", std::span<const float, 4>(&color.elements[0], 4)));
        glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Rect)));
    }
}

void effect::help_box::change_shader(Engine::ShaderName name, float f, bool depar)
{
    shader = name;
    Set_basic_color();
    timer.Set(f);
    depart = depar;
}

void effect::help_box::change_helper(vec4 value)
{
    any_helper = value;
}

void effect::help_box::Set_basic_color()
{
    if (shader == Engine::noise_box) {
        color = (vec4(0.f, 0.f, 0.f, 0.5f));
    }
    else if (shader == Engine::bright) {
        color = (vec4(1.f, 1.f, 1.f, 0.0f));
    }
}


