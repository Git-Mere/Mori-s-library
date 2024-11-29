#include "Engine/Collision.h"
RectCollision::RectCollision(GameObject* object) :
    Collision(object)
{
    collision_box = WorldBoundary();
}


Math::rect RectCollision::WorldBoundary() {
    vec2 result_point_1;
    vec2 result_point_2;

    vec2 pos = object->GetPosition();

    result_point_1.x = pos.x - object->GetCollisionScale().x / 2;
    result_point_1.y = pos.y - object->GetCollisionScale().y / 2;

    result_point_2.x = pos.x + object->GetCollisionScale().x / 2;
    result_point_2.y = pos.y + object->GetCollisionScale().y / 2;


    return { result_point_1, result_point_2 };
}

void RectCollision::Draw(const mat3& world_to_ndc) {
    float tmp_radius = object->GetRotation();
    const mat3 T = mat3::build_translation(object->GetPosition());
    const mat3 R = mat3{ cos(tmp_radius), sin(tmp_radius),0 ,-sin(tmp_radius) ,cos(tmp_radius),0,0,0,1 };
    const mat3 H = mat3::build_scale(WorldBoundary().Size());
    model_to_ndc = T * R * H;
    model_to_ndc = world_to_ndc * model_to_ndc;//

    Engine::Instance()->Get_Shader(Engine::collision).Use();
    Engine::Instance()->Get_Model(Engine::ModelName::Rect).Use();
    glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uFillColor", to_span(color)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("IsCircle", false));
    glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Rect)));
    glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

}

bool RectCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return false;
    }
    
    switch (other_collider->Shape()) {
    case CollisionShape::Circle:
    {
        CircleCollision* tmp = dynamic_cast<CircleCollision*>(other_collider);
        return Circle_Rect(tmp->GetPoint(), tmp->GetRadius(), WorldBoundary());
    }
        break;
    case CollisionShape::Rect:
    {
        RectCollision* tmp = dynamic_cast<RectCollision*>(other_collider);
        return Rect_Rect(WorldBoundary(), tmp->Get_Rect());
    }
        break;
    case CollisionShape::Polygon:
    {
        PolyCollision* tmp = dynamic_cast<PolyCollision*>(other_collider);
        return Rect_Poly(WorldBoundary(), tmp->Get_vertices());
    }
        break;
    case CollisionShape::Line:
    {
        LineCollision* tmp = dynamic_cast<LineCollision*>(other_collider);
        return Line_Rect(tmp->Get_Line(), WorldBoundary());
    }
        break;
    case CollisionShape::Point:
    {
        PointCollision* tmp = dynamic_cast<PointCollision*>(other_collider);
        return Point_Rect(tmp->Get_point(), WorldBoundary());
    }
        break;
    }

    return false;
}

const Math::rect RectCollision::Get_Rect()
{
    return WorldBoundary();
}
