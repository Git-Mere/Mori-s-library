#include "Engine/Collision.h"

LineCollision::LineCollision(GameObject* object, std::vector<vec2> line) : Collision(object), line({ vec2(line[0].x, line[0].y), vec2(line[1].x, line[1].y) })
{
    CreateUsermodel(line);
}

void LineCollision::Draw(const mat3& world_to_ndc)
{
    float angle = object->GetRotation();
    const mat3 T = mat3::build_translation(object->GetPosition());
    const mat3 R = mat3{ cos(angle), sin(angle),0 ,-sin(angle) ,cos(angle),0,0,0,1 };
    const mat3 H = mat3::build_scale(object->GetCollisionScale());
    model_to_ndc = T * R * H;
    model_to_ndc = world_to_ndc * model_to_ndc;

    ///hit range//
    glCheck(glLineWidth(2.0f));
    Engine::Instance()->Get_Shader(Engine::collision).Use();
    model.Use();
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uFillColor", to_span(color)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("IsCircle", false));
    glCheck(GLDrawIndexed(model));
}

bool LineCollision::IsCollidingWith(GameObject* other_object)
{
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return false;
    }
    switch (other_collider->Shape()) {
    case CollisionShape::Circle:
    {
        CircleCollision* tmp = dynamic_cast<CircleCollision*>(other_collider);
        return Line_Circle(Get_Line(), tmp->GetPoint(), tmp->GetRadius());
    }
    break;
    case CollisionShape::Rect:
    {
        RectCollision* tmp = dynamic_cast<RectCollision*>(other_collider);
        return Line_Rect(Get_Line(), tmp->Get_Rect());
    }
    break;
    case CollisionShape::Polygon:
    {
        PolyCollision* tmp = dynamic_cast<PolyCollision*>(other_collider);
        return Line_Poly(Get_Line(), tmp->Get_vertices());
    }
    break;
    case CollisionShape::Line:
    {
        LineCollision* tmp = dynamic_cast<LineCollision*>(other_collider);
        return Line_Line(Get_Line(), tmp->Get_Line());
    }
    break;
    case CollisionShape::Point:
    {
        PointCollision* tmp = dynamic_cast<PointCollision*>(other_collider);
        return Point_Line(tmp->Get_point(), Get_Line());
    }
    break;
    }
    return false;
}

Math::Line LineCollision::Get_Line()
{
    Math::Line result;//
    vec2 point1 = object->GetMatrix() * line.point_1;
    vec2 point2 = object->GetMatrix() * line.point_2;
    result.point_1 = point1;
    result.point_2 = point2;

	return result;
}

void LineCollision::CreateUsermodel(std::vector<vec2> input_vec)
{
    model.SetPrimitivePattern(GLPrimitive::Lines);

    std::vector<vec2> positions = input_vec;
    std::vector<unsigned> indices;
    for (int i = 0; i < positions.size(); i++) {
        indices.push_back(i);
    }

    GLVertexBuffer    position_buffer(std::span{ positions });

    glCheck(model.SetVertexCount(static_cast<int>(positions.size())));

    GLAttributeLayout position_coor;
    position_coor.component_type = GLAttributeLayout::Float;
    position_coor.component_dimension = GLAttributeLayout::_2;
    position_coor.normalized = false;
    position_coor.vertex_layout_location = 0; // 1st field is 0 index based
    position_coor.stride = sizeof(vec2);
    position_coor.offset = 0;
    position_coor.relative_offset = 0;

    model.AddVertexBuffer(std::move(position_buffer), { position_coor });

    GLIndexBuffer       index_buffer(indices);
    glCheck(model.SetIndexBuffer(std::move(index_buffer)));
    
}