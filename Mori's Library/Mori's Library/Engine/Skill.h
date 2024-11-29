#pragma once
#include "Input.h"
#include "Engine.h"
#include "vec2.h"
#include "../Mori/Player.h"
#include "../Mori/Timer.h"

class Skill {
public:
	enum class skill_name {
		Dash,
		Parrying,
		Jump
	};
public:
	Skill(Input::Keys key, Player& player, float cooltime, float delay = 0);
	virtual ~Skill() {};

	bool use();
	bool now_cooltime();
	bool now_delay();
	virtual void Cooling(float dt);
	virtual void func() {};
	virtual skill_name Get_type() = 0;
	virtual void change_config(float cooltime, float delay);
protected:
	Input::Keys key;
	Player* player;
	DownTimer cooltimer;
	DownTimer delay;


protected: //����;;
	void SetPosition(vec2 new_position);
	void SetVelocity(vec2 new_position);
	void SetScale(vec2 new_scale);
	void SetRotation(float new_rotation);
	void SetCollisionScale(vec2 Collision_scale, bool is_chage);
	void SetShaderName(Engine::ShaderName input);

};

class dash : public Skill {
public:
	dash(Input::Keys key, Player& player, float delay, float cooltime, float dash_speed, float invin_time);
	void func() override;
	skill_name Get_type() override {
		return skill_name::Dash;
	}
private:
	float dash_speed;
	float invin_time;
};

class Jump : public Skill {
public:
	Jump(Input::Keys key, Player& player, float cooltime, float height);
	void func() override;

	skill_name Get_type() override {
		return skill_name::Jump;
	}
private:
	float height;
};

class Parrying : public Skill {
public:
	Parrying(Input::Keys key, Player& player, float cooltime, float delay);
	void func() override;
	void Cooling(float dt) override;
	bool Get_now_parring() const;
	void determine_success(GameObject* object);
	void Draw(const mat3& world_to_ndc);
	void success();
	skill_name Get_type() override {
		return skill_name::Parrying;
	}
private:
	vec2 adjust_value{};
	vec2 ui_size{ Engine::Instance()->Get_Window().x / 5, Engine::Instance()->Get_Window().x / 5 };
	Sprite* effect;
	int direction = 4;
	bool now_parrying = false;
	bool parrying_sound = false;
};