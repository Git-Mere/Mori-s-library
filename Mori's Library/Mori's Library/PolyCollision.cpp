#include "Engine/Collision.h"

PolyCollision::PolyCollision(GameObject* object, std::vector<vec2> vertices)  :  Collision(object), vertices(vertices)
{
    CreateUsermodel(vertices);
}

void PolyCollision::Draw(const mat3& world_to_ndc)
{
    model_to_ndc = world_to_ndc * object->GetMatrix();

    ///hit range//
    glCheck(glLineWidth(2.0f));
    Engine::Instance()->Get_Shader(Engine::collision).Use();
    model.Use();
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uFillColor", to_span(color)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("IsCircle", false));
    glCheck(GLDrawIndexed(model));
}

bool PolyCollision::IsCollidingWith(GameObject* other_object)
{
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return false;
    }
    switch (other_collider->Shape()) {
    case CollisionShape::Circle:
    {
        CircleCollision* tmp = dynamic_cast<CircleCollision*>(other_collider);
        return Circle_Poly(tmp->GetPoint(), tmp->GetRadius(), Get_vertices());
    }
    break;
    case CollisionShape::Rect:
    {
        RectCollision* tmp = dynamic_cast<RectCollision*>(other_collider);
        return Rect_Poly(tmp->Get_Rect(), Get_vertices());
    }
    break;
    case CollisionShape::Polygon:
    {
        PolyCollision* tmp = dynamic_cast<PolyCollision*>(other_collider);
        return Poly_Poly(Get_vertices(), tmp->Get_vertices());
    }
    break;
    case CollisionShape::Line:
    {
        LineCollision* tmp = dynamic_cast<LineCollision*>(other_collider);
        return Line_Poly(tmp->Get_Line(), Get_vertices());
    }
    break;
    case CollisionShape::Point:
    {
        PointCollision* tmp = dynamic_cast<PointCollision*>(other_collider);
        return Point_Poly(tmp->Get_point(), Get_vertices());
    }
    break;
    }
    return false;
}

const std::vector<vec2> PolyCollision::Get_vertices()
{
    std::vector<vec2> result;
    for (int i = 0; i < vertices.size(); i++) {
        result.push_back(object->GetMatrix() * vertices[i]);
    }

    return result;
}

void PolyCollision::CreateUsermodel(std::vector<vec2> input_vec)
{
    model.SetPrimitivePattern(GLPrimitive::LineLoop);

    std::vector<vec2> positions = input_vec;
    std::vector<unsigned> indices;
    size_t size = positions.size();
    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            indices.push_back(i);
            indices.push_back(0);//
        }
        else {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
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
