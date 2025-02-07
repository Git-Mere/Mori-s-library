
#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Rect.h"
#include "Matrix.h"
namespace Math {
	class TransformationMatrix;
}

class GameObject;
class Collision : public Component {
public:
	enum class CollisionShape {
		Rect,
		Circle
	};
	enum kind_of_collision {
		No_collision,
		collision,
		can_parrying
	};
	virtual CollisionShape Shape() = 0;
	virtual void Draw(const mat3& world_to_ndc) = 0;
	virtual int IsCollidingWith(GameObject* other_object) = 0;
	virtual bool IsCollidingWith(vec2 point) = 0;
protected:
	color3      color{ 1,0,1 };
	mat3 model_to_ndc;
	static constexpr float range_of_parrying = 100.f;
};
//////////////////////////////////////// Rect Collision ////////////////////////////////////////
class RectCollision : public Collision {
public:
	RectCollision(GameObject* object);
	CollisionShape Shape() override {
		return CollisionShape::Rect;
	}
	void Draw(const mat3& world_to_ndc) override;
	int IsCollidingWith(GameObject* other_object) override;
	bool IsCollidingWith(vec2 point) override;
	Math::rect WorldBoundary();
private:
	GameObject* object;
};
//////////////////////////////////////// Circle Collision ////////////////////////////////////////
class CircleCollision : public Collision {
public:
	CircleCollision(GameObject* object);
	CollisionShape Shape() override {
		return CollisionShape::Circle;
	}
	void Draw(const mat3& world_to_ndc) override;
	int IsCollidingWith(GameObject* other_object) override;
	bool IsCollidingWith(vec2 point) override;
	float GetRadius();
	GameObject* GetPtr() { return object; }
private:
	GameObject* object;
};

