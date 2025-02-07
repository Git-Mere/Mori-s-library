#include "CameraCom.h"
#include "Engine.h"


CameraComponent::CameraComponent(vec2 position, ivec2 map_size, float move_range) 
	: player_zone({ Engine::Instance()->Get_Window() * -move_range, Engine::Instance()->Get_Window() * move_range }), limit({ -map_size, map_size })
{
	camera.Position = position;
	camera.SetOrientation(0);
	vec2 tmp = Engine::Instance()->Get_Window();
	camera_view.SetFramebufferSize(static_cast<int>(tmp.x), static_cast<int>(tmp.y));
}

const mat3 CameraComponent::Getworld_to_ndc() const
{
	const mat3 world_to_cam = camera.BuildWorldToCamera();
	const mat3 cam_to_ndc = camera_view.BuildCameraToNDC();
	const mat3 world_to_ndc = cam_to_ndc * world_to_cam;
	return world_to_ndc;
}

const mat3 CameraComponent::Getworld_to_ndc_don_move() const
{
	const mat3 cam_to_ndc = camera_view.BuildCameraToNDC();
	const mat3 world_to_ndc = cam_to_ndc;
	return world_to_ndc;
}

void CameraComponent::Update(const vec2& player_position) {
	if (player_position.x > player_zone.Right() + camera.Position.x) {
		camera.Position.x = player_position.x - player_zone.Right();
	}
	if (player_position.x - camera.Position.x < player_zone.Left()) {
		camera.Position.x = player_position.x - player_zone.Left();
	}
	if (player_position.y > player_zone.Top() + camera.Position.y) {
		camera.Position.y = player_position.y - player_zone.Top();
	}
	if (player_position.y - camera.Position.y < player_zone.Bottom()) {
		camera.Position.y = player_position.y - player_zone.Bottom();
	}

	if (camera.Position.x < limit.Left()) {
		camera.Position.x = static_cast<float>(limit.Left());
	}
	if (camera.Position.x > limit.Right()) {
		camera.Position.x = static_cast<float>(limit.Right());
	}
	if (camera.Position.y < limit.Bottom()) {
		camera.Position.y = static_cast<float>(limit.Bottom());
	}
	if (camera.Position.y > limit.Top()) {
		camera.Position.y = static_cast<float>(limit.Top());
	}
	vec2 window = Engine::Instance()->Get_Window();

	destroy_rect = { camera.Position - window, camera.Position + window };
}

void CameraComponent::SetPosition(vec2 new_position) {
	camera.Position = new_position;
}

const vec2& CameraComponent::GetPosition() const {
	return camera.Position;
}

const Math::irect& CameraComponent::Getlimit() const
{
	return limit;
}

void CameraComponent::SetLimit(Math::irect new_limit) {
	limit = new_limit;
}

bool CameraComponent::obj_destory(const vec2& player_position)
{
	if (player_position.x < destroy_rect.Left() || player_position.x > destroy_rect.Right()
		|| player_position.y > destroy_rect.Top() || player_position.y < destroy_rect.Bottom()) {
		return true;
	}
	return false;
}
