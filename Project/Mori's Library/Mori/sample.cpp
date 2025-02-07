#include "sample.h"
#include "States.h"

sample::sample(vec2 position) : GameObject(position)
{
	/////거의 필수
	AddGOComponent(new Sprite("assets/sprite/sample.spt", this));
	SetScale(vec2(50));

	//////선택 사항
	Setcolor(color3{ 0.0f, 0.68f, 0.94f });
	SetShaderName(Engine::noimage);
}

bool sample::CanCollideWith(GameObjectTypes other_object_type)
{
	if (other_object_type == GameObjectTypes::Player) {
		return true;
	}
	return false;
}

