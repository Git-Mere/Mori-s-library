#include "Engine/Collision.h"

PointCollision::PointCollision(GameObject* object) : Collision(object)
{
	point = object->GetPosition();
}

void PointCollision::Draw([[maybe_unused]] const mat3& world_to_ndc)
{
}

bool PointCollision::IsCollidingWith(GameObject* other_object)
{
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return false;
    }
    switch (other_collider->Shape()) {
    case CollisionShape::Circle:
    {
        CircleCollision* tmp = dynamic_cast<CircleCollision*>(other_collider);
        return Point_Circle(Get_point(), tmp->GetPoint(), tmp->GetRadius());
    }
    break;
    case CollisionShape::Rect:
    {
        RectCollision* tmp = dynamic_cast<RectCollision*>(other_collider);
        return Point_Rect(Get_point(), tmp->Get_Rect());
    }
    break;
    case CollisionShape::Polygon:
    {
        PolyCollision* tmp = dynamic_cast<PolyCollision*>(other_collider);
        return Point_Poly(Get_point(), tmp->Get_vertices());
    }
    break;
    case CollisionShape::Line:
    {
        LineCollision* tmp = dynamic_cast<LineCollision*>(other_collider);
        return Point_Line(Get_point(), tmp->Get_Line());
    }
    break;
    case CollisionShape::Point:
    {
        PointCollision* tmp = dynamic_cast<PointCollision*>(other_collider);
        return Point_Point(Get_point(), tmp->Get_point());
    }
    break;
    }
    return false;
}

const vec2& PointCollision::Get_point()
{
	return object->GetPosition();
}
