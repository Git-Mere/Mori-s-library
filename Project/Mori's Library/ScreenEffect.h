#pragma once
#include "Mori/Timer.h"
#include "Engine/CameraCom.h"
#include "Engine/GameObject.h"
#include "UIManager.h"
namespace effect {
	class Shake : public Component {
	public:
		Shake(CameraComponent& camera, float time, float how_strong = 10, int direction = 1);
		void Update(float) override;
		void shake();
		void Set_info(float how_long, int limit, float how_strong = 10, int compass = 1);
	private:
		CameraComponent* camera;
		DownTimer timer;
		float how_strong;
		int direction;
		bool dir = false;

		int shake_limit = 10;
		int shake_count = 0;
	};

	class help_box : public UI {
	public:
		help_box(Engine::ShaderName name);
		void Update(float dt) override;
		void Draw(const mat3& world_to_ndc) override;
		void change_shader(Engine::ShaderName name, float f, bool depar = true);
		void change_helper(vec4 value);
		UITypes Type() override { return UITypes::ScreenEffect; }
	private:
		void Set_basic_color();
		vec4				any_helper{};
		vec4				color{};
		Engine::ShaderName	shader;
		UpTimer				timer;
		bool depart = true;
	};
}