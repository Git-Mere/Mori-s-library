

#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Matrix.h"
#include "Component.h"
#include <CameraView.h>
#include <Camera.h>

class CameraComponent : public Component {
public:	
	//position - 카메라의 위치
	//map_size - 맵의 크기 결정
	//move_range - 플레이어 존의 크기 결정
	CameraComponent(vec2 position, ivec2 map_size, float move_range = 0.05f);
	const mat3 Getworld_to_ndc() const;
	const mat3 Getworld_to_ndc_don_move() const;
	void Update(const vec2& player_position);
	void SetPosition(vec2 new_position);
	const vec2& GetPosition() const;
	const Math::irect& Getlimit() const;
	void SetLimit(Math::irect new_limit);
	
	bool obj_destory(const vec2& player_position);
	//Math::TransformationMatrix GetMatrix();
private:
	Camera     camera{};
	CameraView camera_view{};
	float      move_scalar = 0;
	float      turn_scalar = 0;
	float      strafe_scalar = 0;
	float      move_speed = 120.0f;

	Math::irect limit;
	Math::rect player_zone;
	Math::rect destroy_rect;
};