#pragma once
#include "../Engine/GameObject.h"

class sample : public GameObject {
public:
    ///////////////////////////////////////////////////////////////////////////////////////////
    //������Ʈ ����� ������ �־�� �ϴ� �Լ�. �� ���� �ٲ��ְ� GameObjectTypes.h �� �ݵ�� �߰��ؾ���////
    ///////////////////////////////////////////////////////////////////////////////////////////
    sample(vec2 position);
    GameObjectTypes Type() override { return GameObjectTypes::sample; }
    std::string TypeName() override { return "sample"; }

    ///////////////////////////////////////////////////////////////////////////////////////////
    ////////////////            �ʿ信 ���� �����ؾ� �ϴ� �Լ�           ///////////////////////
    //////////////////   �ݵ�� GameObject.h �� �ִ� ������ ������� ��. ////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    bool CanCollideWith(GameObjectTypes other_object_type);
    void ResolveCollision([[maybe_unused]] GameObject* other_object) {};
    void Draw(const mat3& world_to_ndc);
    void Update(float dt);
private:
    //������ �ʿ��ϰڴٰ� ������ ������ �߰�
};