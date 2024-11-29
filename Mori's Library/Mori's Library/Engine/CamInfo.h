#pragma once
#include <Camera.h>
#include <CameraView.h>
#include "GameObject.h"
struct CamInfo
{
	GameObject  obj;
	Camera     camera{};
	CameraView camera_view{};
	float      move_scalar = 0;
	float      turn_scalar = 0;
	float      strafe_scalar = 0;
	float      move_speed = 120.0f;
};