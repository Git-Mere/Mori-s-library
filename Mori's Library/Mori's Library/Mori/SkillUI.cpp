#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "SkillUI.h"
#include "../Engine/Skill.h"

SkillUI::SkillUI(Skill& skill, int count) : skill(&skill) {
    vec2 pos = Engine::Instance()->Get_Window() / 2;
    vec2 ui_size{ Engine::Instance()->Get_Window().x / 15, Engine::Instance()->Get_Window().x / 15 };

    const mat3 T = mat3::build_translation(-pos + ui_size + vec2((ui_size.x + 30.f) * count, 0));
    const mat3 H = mat3::build_scale(ui_size);
    matrix = T * H;

    decide_sprite();
}

void SkillUI::Update([[maybe_unused]] float dt)
{
    if (skill->now_cooltime() == false) {
        sprite->PlayAnimation(static_cast<int>(Animations::Ready));
    }
    else {
        sprite->PlayAnimation(static_cast<int>(Animations::COOLTIME));
    }
}

void SkillUI::Draw(const mat3& camera_matrix)
{
    mat3 final_matrix = camera_matrix * matrix;
    sprite->No_Gameobject_Draw(final_matrix);
}

void SkillUI::decide_sprite()
{
    switch (skill->Get_type()) {
    case Skill::skill_name::Dash:
        sprite = (new Sprite("assets/sprite/DashUI.spt", this));
        break;
    case Skill::skill_name::Jump:
        sprite = (new Sprite("assets/sprite/DashUI.spt", this));
        break;
    case Skill::skill_name::Parrying:
        sprite = (new Sprite("assets/sprite/ParryingUI.spt", this));
        break;
    }

}

