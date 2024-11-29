
#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Rect.h"
#include "Matrix.h"

namespace
{
	std::span<const float, 3 * 3> to_span(const mat3& m)
	{
		return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
	}
	std::span<const float, 3> to_span(const color3& c)
	{
		return std::span<const float, 3>(&c.elements[0], 3);
	}
}

class GameObject;
class Collision : public Component {
public:
	enum class CollisionShape {
		Rect,
		Circle,
		Polygon,
		Line,
		Point
	};

public:
	Collision(GameObject* object) : object(object) {};
	virtual CollisionShape Shape() = 0;
	virtual void Draw(const mat3& world_to_ndc) = 0;
	virtual bool IsCollidingWith(GameObject* other_object) = 0;
protected:
	color3      color;
	static constexpr color3  yes_color{ 1,1,0 };
	static constexpr color3  no_color{1,0,1};
	mat3 model_to_ndc;
	GameObject* object;

	void Set_color(const vec3& c);

	bool Point_Point(vec2 point1, vec2 point2);
	bool Point_Line(vec2 point1, Math::Line line);
	bool Point_Circle(vec2 point1, vec2 circle_point, float radius);
	bool Point_Rect(vec2 point1, Math::rect rect1);
	bool Point_Poly(vec2 point1, std::vector<vec2> vertices);

	bool Line_Line(Math::Line line1, Math::Line line2);
	bool Line_Circle(Math::Line line1, vec2 circle_point, float radius);
	bool Line_Rect(Math::Line line1, Math::rect rect1);
	bool Line_Poly(Math::Line line1, std::vector<vec2> vertices);

	bool Circle_Circle(vec2 circle_point1, float radius1, vec2 circle_point2, float radius2);
	bool Circle_Rect(vec2 circle_point1, float radius1, Math::rect rect1);
	bool Circle_Poly(vec2 circle_point1, float radius1, std::vector<vec2> vertices);

	bool Rect_Rect(Math::rect rect1, Math::rect rect2);
	bool Rect_Poly(Math::rect rect1, std::vector<vec2> vertices);

	bool Poly_Poly(std::vector<vec2> vertices, std::vector<vec2> vertices2);
};

class RectCollision : public Collision {
public:
	RectCollision(GameObject* object);
	void Draw(const mat3& world_to_ndc) override;
	bool IsCollidingWith(GameObject* other_object) override;
	const Math::rect Get_Rect();

	CollisionShape Shape() override {
		return CollisionShape::Rect;
	}
private:
	Math::rect WorldBoundary();
	Math::rect collision_box;
};

class CircleCollision : public Collision {
public:
	CircleCollision(GameObject* object);
	void Draw(const mat3& world_to_ndc) override;
	bool IsCollidingWith(GameObject* other_object) override;
	const float GetRadius();
	const vec2& GetPoint();

	CollisionShape Shape() override {
		return CollisionShape::Circle;
	}
private:
	float makeRadius();
	vec2 circle_point;
	float radius;
};

class PolyCollision : public Collision {
public:
	PolyCollision(GameObject* object, std::vector<vec2> vertices);
	void Draw(const mat3& world_to_ndc) override;
	bool IsCollidingWith(GameObject* other_object) override;
	const std::vector<vec2> Get_vertices();
	void CreateUsermodel(std::vector<vec2> input_vec);

	CollisionShape Shape() override {
		return CollisionShape::Polygon;
	}
private:
	std::vector<vec2> vertices;
	GLVertexArray model;
};

class LineCollision : public Collision {
public:
	LineCollision(GameObject* object, std::vector<vec2> line);
	void Draw(const mat3& world_to_ndc) override;
	bool IsCollidingWith(GameObject* other_object) override;
	Math::Line Get_Line();
	void CreateUsermodel(std::vector<vec2> input_vec);

	CollisionShape Shape() override {
		return CollisionShape::Line;
	}
private:
	Math::Line line;
	GLVertexArray model;
};

class PointCollision : public Collision {
public:
	PointCollision(GameObject* object);
	void Draw(const mat3& world_to_ndc) override;
	bool IsCollidingWith(GameObject* other_object) override;
	const vec2& Get_point();

	CollisionShape Shape() override {
		return CollisionShape::Point;
	}
private:
	vec2 point;
};