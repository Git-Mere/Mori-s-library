#pragma once
#include "../Engine/GameObject.h"
#include "../UIManager.h"

class Skill;

class SkillUI : public UI {
public:
    SkillUI(Skill& skill, int count);
    void Update(float dt);
    void Draw(const mat3& camera_matrix) override;
    UITypes Type() { return UITypes::SkillUI; };
private:
    
    Skill* skill;
    enum class Animations {
        Ready,
        COOLTIME
    };

    void decide_sprite();
};