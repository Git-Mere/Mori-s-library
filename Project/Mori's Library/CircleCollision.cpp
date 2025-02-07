#include "Engine/Collision.h"


CircleCollision::CircleCollision(GameObject* object) : Collision(object)
{
    circle_point = object->GetPosition();
    radius = makeRadius();
}

void CircleCollision::Draw(const mat3& world_to_ndc)
{
    float angle = object->GetRotation();
    const mat3 T = mat3::build_translation(object->GetPosition());
    const mat3 R = mat3{ cos(angle), sin(angle),0 ,-sin(angle) ,cos(angle),0,0,0,1 };
    const mat3 H = mat3::build_scale(GetRadius());
    model_to_ndc = T * R * H;
    model_to_ndc = world_to_ndc * model_to_ndc;

    ///hit range//
    Engine::Instance()->Get_Shader(Engine::collision).Use();
    Engine::Instance()->Get_Model(Engine::ModelName::Circle).Use();
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uFillColor", to_span(color)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("IsCircle", true));
    glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Circle)));
}

bool CircleCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return false;
    }
    switch (other_collider->Shape()) {
    case CollisionShape::Circle:
    {
        CircleCollision* tmp = dynamic_cast<CircleCollision*>(other_collider);
        return Circle_Circle(GetPoint(), GetRadius(), tmp->GetPoint(), tmp->GetRadius());
    }
    break;
    case CollisionShape::Rect:
    {
        RectCollision* tmp = dynamic_cast<RectCollision*>(other_collider);
        return Circle_Rect(GetPoint(), GetRadius(), tmp->Get_Rect());
    }
    break;
    case CollisionShape::Polygon://
    {
        PolyCollision* tmp = dynamic_cast<PolyCollision*>(other_collider);
        return Circle_Poly(GetPoint(), GetRadius(), tmp->Get_vertices());
    }
    break;
    case CollisionShape::Line:
    {
        LineCollision* tmp = dynamic_cast<LineCollision*>(other_collider);
        return Line_Circle(tmp->Get_Line(), GetPoint(), GetRadius());
    }
    break;
    case CollisionShape::Point:
    {
        PointCollision* tmp = dynamic_cast<PointCollision*>(other_collider);
        return Point_Circle(tmp->Get_point(), GetPoint(), GetRadius());
    }
    break;
    }
    return false;
}


float CircleCollision::makeRadius()
{
    return abs(object->GetCollisionScale().x / 2);
}

const float CircleCollision::GetRadius() 
{
    return abs(object->GetCollisionScale().x / 2);;
}

const vec2& CircleCollision::GetPoint()
{
    return object->GetPosition();
}
