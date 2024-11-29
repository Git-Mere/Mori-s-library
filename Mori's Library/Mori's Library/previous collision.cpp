

#include "Collision.h"
#include "Rect.h"
#include "Engine.h"

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

RectCollision::RectCollision(GameObject* object) :
    object(object)
{
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
    model_to_ndc = world_to_ndc * model_to_ndc;

    //shader.Use();
    Engine::Instance()->Get_Shader(Engine::collision).Use();
    //model.Use();
    Engine::Instance()->Get_Model(Engine::ModelName::Rect).Use();
    glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uFillColor", to_span(color)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
    glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("IsCircle", false));
    glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Rect)));
    glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    if (object->Type() == GameObjectTypes::Player) {
        const mat3 H2 = mat3::build_scale(WorldBoundary().Size() + range_of_parrying);
        model_to_ndc = T * R * H2;
        model_to_ndc = world_to_ndc * model_to_ndc;
        Engine::Instance()->Get_Model(Engine::ModelName::Rect).Use();
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
        glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Rect)));
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

}

int RectCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return kind_of_collision::No_collosion;
    }
    if (other_collider->Shape() == CollisionShape::Circle) {
        CircleCollision* cir{ dynamic_cast<CircleCollision*>(other_collider) };
        float radi = cir->GetRadius();
        Math::rect rec = WorldBoundary();
        float cx = cir->GetPtr()->GetPosition().x;
        float cy = cir->GetPtr()->GetPosition().y;
        float testX = cx;
        float testY = cy;

        // which edge is closest?//
        if (cx < rec.Left()) { testX = rec.Left(); }     // test left edge
        else if (cx > rec.Right()) { testX = rec.Right(); }   // right edge
        if (cy < rec.Bottom()) { testY = rec.Bottom(); }      // top edge
        else if (cy > rec.Top()) { testY = rec.Top(); }  // bottom edge

        // get distance from closest edges
        float distX = cx - testX;
        float distY = cy - testY;
        float distance = (distX * distX) + (distY * distY);
        float check_radius = radi * radi;

        // if the distance is less than the radius, collision!
        float parrying_ragne = (radi + range_of_parrying) * (radi + range_of_parrying);
        //
        if (distance <= parrying_ragne && distance > check_radius) {
            return kind_of_collision::can_parrying;
        }

        if (distance <= check_radius) {
            return kind_of_collision::collision;
        }
    }
    else if (other_collider->Shape() == CollisionShape::Rect) {
        Math::rect rectangle_1 = WorldBoundary();
        Math::rect parrying_rectangle = Math::rect{ rectangle_1.point_1 + range_of_parrying, rectangle_1.point_2 - range_of_parrying };
        Math::rect rectangle_2 = dynamic_cast<RectCollision*>(other_collider)->WorldBoundary();

        bool inandout = false;
        if (rectangle_1.Left() < rectangle_2.Right() && rectangle_1.Right() > rectangle_2.Left()
            && rectangle_1.Top() > rectangle_2.Bottom() && rectangle_1.Bottom() < rectangle_2.Top()) {
            inandout = true;
        }

        if (inandout == false && parrying_rectangle.Left() < rectangle_2.Right() && parrying_rectangle.Right() > rectangle_2.Left()
            && parrying_rectangle.Top() > rectangle_2.Bottom() && parrying_rectangle.Bottom() < rectangle_2.Top()) {
            return kind_of_collision::can_parrying;
        }
        if (inandout) {
            return kind_of_collision::collision;
        }
    }
    return kind_of_collision::No_collosion;
}

bool RectCollision::IsCollidingWith(vec2 point)
{
    Math::rect rectangle_1 = WorldBoundary();

    if (rectangle_1.Left() <= point.x && rectangle_1.Right() >= point.x
        && rectangle_1.Top() >= point.y && rectangle_1.Bottom() <= point.y) {
        return true;
    }
    return false;
}

//////////////////////////////////////////// Circle ////////////////////////////////////////////
CircleCollision::CircleCollision(GameObject* object) : object(object)
{
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

    ///parrying range
    if (object->Type() == GameObjectTypes::Player) {
        const mat3 H2 = mat3::build_scale(GetRadius() + (range_of_parrying));
        model_to_ndc = T * R * H2;
        model_to_ndc = world_to_ndc * model_to_ndc;
        Engine::Instance()->Get_Model(Engine::ModelName::Circle).Use();
        glCheck(Engine::Instance()->Get_Shader(Engine::collision).SendUniform("uModelToNDC", to_span(model_to_ndc)));
        glCheck(GLDrawIndexed(Engine::Instance()->Get_Model(Engine::ModelName::Circle)));
    }
}

int CircleCollision::IsCollidingWith(GameObject* other_object) {
    Collision* other_collider = other_object->GetGOComponent<Collision>();
    if (other_collider == nullptr) {
        return false;
    }
    if (other_collider->Shape() == CollisionShape::Rect) {
        // temporary variables to set edges for testing
        float cx = GetPtr()->GetPosition().x;
        float cy = GetPtr()->GetPosition().y;
        float testX = cx;//
        float testY = cy;
        Math::rect rec{ dynamic_cast<RectCollision*>(other_collider)->WorldBoundary() };

        // which edge is closest?
        if (cx < rec.Left()) { testX = rec.Left(); }     // test left edge
        else if (cx > rec.Right()) { testX = rec.Right(); }   // right edge
        if (cy < rec.Bottom()) { testY = rec.Bottom(); }      // top edge
        else if (cy > rec.Top()) { testY = rec.Top(); }  // bottom edge

        // get distance from closest edges
        float radi = GetRadius();
        float distX = cx - testX;
        float distY = cy - testY;
        float distance = (distX * distX) + (distY * distY);
        float check_radius = radi * radi;

        // if the distance is less than the radius, collision!
        float tmp = range_of_parrying / 2;
        float parrying_ragne = (radi + tmp) * (radi + tmp);

        if (distance <= parrying_ragne && distance > check_radius) {
            return kind_of_collision::can_parrying;
        }

        if (distance <= check_radius) {
            return kind_of_collision::collision;
        }
    }
    else if (other_collider->Shape() == CollisionShape::Circle) {
        float radius_1 = GetRadius();
        float radius_2 = dynamic_cast<CircleCollision*>(other_collider)->GetRadius();

        float distX = other_object->GetPosition().x - object->GetPosition().x;
        float distY = other_object->GetPosition().y - object->GetPosition().y;

        float Distance = (distX * distX) + (distY * distY);
        float RadiusSum = (radius_1 + radius_2) * (radius_1 + radius_2);
        float parrying_ragne = (radius_1 + radius_2 + range_of_parrying) * (radius_1 + radius_2 + range_of_parrying);


        if (Distance <= parrying_ragne && Distance > RadiusSum) {
            return kind_of_collision::can_parrying;
        }

        if (Distance <= RadiusSum) {
            return kind_of_collision::collision;
        }
    }
    return kind_of_collision::No_collosion;
}

bool CircleCollision::IsCollidingWith(vec2 point)
{
    float radius_1 = GetRadius();

    double distX = object->GetPosition().x - point.x;
    double distY = object->GetPosition().y - point.y;

    double Distance = (distX * distX) + (distY * distY);
    double RadiusSum = radius_1 * radius_1;

    if (Distance <= RadiusSum) {
        return true;
    }
    return false;
}

float CircleCollision::GetRadius()
{
    return abs(object->GetCollisionScale().x / 2);
}
