
#include "Background.h"
#include "../Engine/Engine.h"

void Background::Add(const std::filesystem::path& texturePath, float speed, vec2 scale, vec2 position) {

	backgrounds.push_back({ new Sprite(texturePath), speed, scale, position});
}
void Background::Update(float dt)
{
	int size = static_cast<int>(backgrounds.size());
	for (int i = size - 1; i >= 0; i--) {
		backgrounds[i].texture->Update(dt);
	}
}
void Background::Unload() {
	for (auto a : backgrounds) {
		delete a.texture;
	}
	backgrounds.clear();
}

void Background::Draw(const CameraComponent& camera) {
	int size = static_cast<int>(backgrounds.size());
	for (int i = size-1; i >= 0; i--) {
		//float pos_x = camera.GetPosition().x * backgrounds[i].speed;
		//float pos_y = camera.GetPosition().y * backgrounds[i].speed;
		//vec2 a = {pos_x,  pos_y };

		const mat3 T = mat3::build_translation(backgrounds[i].position);
		//const mat3 R = mat3{ cos(GetRotation()), sin(GetRotation()),0 ,-sin(GetRotation()) ,cos(GetRotation()),0,0,0,1 };
		const mat3 H = mat3::build_scale(backgrounds[i].scale);

		mat3 model_to_ndc = T * H;
		mat3 final_matrix = camera.Getworld_to_ndc() * model_to_ndc;
		backgrounds[i].texture->No_Gameobject_Draw(final_matrix);
	}
}

ivec2 Background::GetSize() {
	return backgrounds[0].texture->GetFrameSize();
}
