#include "captain.h"

Captain::Captain(vec2 position) : GameObject(position)
{
	AddGOComponent(new Sprite("assets/sprite/Book3/Captain.spt", this));
	SetScale(vec2(600, 600));
	//Setcolor(vec4(0.5f, 0.6f, 0.7f, 1.f));
	//SetShaderName(Engine::noimage);
}

void Captain::middle_attack(float speed)
{
	SetPosition(vec2(0, 590));
	SetVelocity(vec2(0, speed));
}

void Captain::parring(vec2 speed)
{
	SetVelocity(speed);
}

void Captain::initialize()
{
	SetPosition(vec2(100, 200));
	SetVelocity(vec2(0, 0));
}

