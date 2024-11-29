/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Camera.cpp
Project:     Engine
Author:     Jonathan Holmes, Seungheon Jeon
Created:    March 8, 2023
Updated:    May 31, 2023
*/

#include "Camera.h"
Camera::Camera(Math::rect player_zone) : player_zone(player_zone), position({ 0,0 }) {}

void Camera::SetPosition(Math::vec2 new_position) {
	position = new_position;
}

const Math::vec2& Camera::GetPosition() const {
	return position;
}

Math::TransformationMatrix Camera::GetMatrix() {
	return Math::TranslationMatrix(-position);
}


void Camera::SetLimit(Math::irect new_limit) {
	limit = new_limit;
}

void Camera::Update(const Math::vec2& player_position) {
	if (player_position.x > player_zone.Right() + position.x) {
		position.x = player_position.x - player_zone.Right();
	}
	if (player_position.x - position.x < player_zone.Left()) {
		position.x = player_position.x - player_zone.Left();
	}

	if (position.x < limit.Left()) {
		position.x = limit.Left();
	}
	if (position.x > limit.Right()) {
		position.x = limit.Right();
	}
	if (position.y < limit.Bottom()) {
		position.y = limit.Bottom();
	}
	if (position.y > limit.Top()) {
		position.y = limit.Top();
	}
}
