/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Camera.h
Project:     Engine
Author:     Jonathan Holmes, Seungheon Jeon
Created:    March 8, 2023
Updated:    May 31, 2023
*/

#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Matrix.h"
#include "Component.h"

class Camera : public Component {
public:
	Camera(Math::rect player_zone);
	void SetPosition(Math::vec2 new_position);
	const Math::vec2& GetPosition() const;
	void SetLimit(Math::irect new_limit);
	void Update(const Math::vec2& player_position);
	Math::TransformationMatrix GetMatrix();
private:
	Math::irect limit;
	Math::vec2 position;
	Math::rect player_zone;
};

