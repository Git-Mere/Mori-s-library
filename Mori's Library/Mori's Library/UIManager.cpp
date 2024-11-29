#include "UIManager.h"
#include "../Mori/SkillUI.h"
#include "Mori/Life.h"
#include "ScreenEffect.h"

UIManager::UIManager(const mat3& camera, Player& player) : dont_move_camera_matrix(camera), player_(&player)
{
    UIs.emplace_back(player_->GetGOComponent<Life>());

    if (player.Get_Skill(0) != nullptr) {
        UIs.emplace_back(new SkillUI(*player_->Get_Skill(0), 0));
    }
    if (player.Get_Skill(1) != nullptr) {
        UIs.emplace_back(new SkillUI(*player_->Get_Skill(1), 1));
    }

    UIs.push_back(new effect::help_box(Engine::bright));
}

UIManager::~UIManager()
{
    size_t size = UIs.size() - 1;
    for (int i = 1; i < size; i++) {
        delete UIs[i];
    }
    UIs.clear();
}

void UIManager::Add(UI* ui) {
    UIs.push_back(ui);
}

void UIManager::Update(float dt) {
    for (UI* a : UIs) {
        a->Update(dt);
    }
}

void UIManager::AllDraw() {
    for (UI* a : UIs) {
        a->Draw(dont_move_camera_matrix);
    }
}

void UIManager::SE_setting(Engine::ShaderName name, float f, bool depar)
{
    GetObject<effect::help_box>()->change_shader(name, f, depar);
}

