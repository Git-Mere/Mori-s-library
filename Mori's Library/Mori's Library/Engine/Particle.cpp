
#include "Particle.h"

Particle::Particle(const std::filesystem::path& sprite_file)
	: GameObject({ 0,0 }), life(0.0)
{
	AddGOComponent(new Sprite(sprite_file, this));
}

bool Particle::CanCollideWith([[maybe_unused]]GameObjectTypes other_object_type) {

	return false;
	
}
void Particle::Set_Rotation(float value)
{
	SetRotation(value);
}
;

void Particle::Start(vec2 p_position, vec2 p_velocity, double max_life)
{
	SetPosition(p_position);
	SetVelocity(p_velocity);
	life = max_life;
	GetGOComponent<Sprite>()->PlayAnimation(0);
}

void Particle::Update(float dt)
{
	if (Alive()) {
		life -= dt;
		//UpdatePosition(GetVelocity() * dt);
		GameObject::Update(dt);
	}
	else {
		SetPosition({ -2000, -2000 });
	}
}

void Particle::Draw(const mat3& world_to_ndc)
{
	if (Alive()) {
		GameObject::Draw(world_to_ndc);
	}
}
