#include "wall.h"
#include "Engine/Collision.h"

Wall::Wall(vec2 position, vec2 scale, std::string sprite_name, float rotate) : GameObject(position, rotate, scale)
{
	AddGOComponent(new Sprite("assets/sprite/Wall/" + sprite_name +".spt", this));
	//
	if (sprite_name == "Bookshelf" || sprite_name == "base_Bookshelf1") {
		SetCollisionScale({ scale.x * 0.6f, scale.y * 0.8f}, true);
	}
	else if (sprite_name == "Long_Wall") {
		SetCollisionScale({ scale.x * 0.2f, scale.y }, true);
	}
	else if (sprite_name == "Long_Wall2") {
		SetCollisionScale({ scale.x * 0.9f, scale.y * 0.3f }, true);
	}
	else if (sprite_name == "ShortBookshelf") {
		SetCollisionScale({ scale.x * 0.2f, scale.y }, true);
	}
	else if (sprite_name == "table") {
		SetCollisionScale({ scale.x * 0.5f, scale.y * 0.5f }, true);
	}
	else if (sprite_name == "coffee_table") {
		SetCollisionScale({ scale.x * 0.5f, scale.y * 0.5f }, true);
	}
	else if (sprite_name == "None") {
		SetShaderName(Engine::noimage);
		Setcolor(vec4(0,0,0,1));
	}

}

bool Wall::CanCollideWith(GameObjectTypes other_object_type)
{
	if (other_object_type == GameObjectTypes::Player) {
		return collision_on;
	}
	return false;
}

void Wall::Set_Collision(bool value)
{
	collision_on = value;
}
