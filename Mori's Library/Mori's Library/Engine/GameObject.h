
#pragma once
#include "Sprite.h"
#include "ComponentManager.h"
#include <color3.h>
#include <mat3.h>
#include "../Mori/GameObjectTypes.h"
#include "Engine.h"


namespace Math { class TransformationMatrix; }

enum class GameObjectTypes;

class Component;

class GameObject {
	friend class Sprite;
public:
	GameObject();
	GameObject(vec2 position);
	GameObject(vec2 position, float rotation, vec2 scale);
	virtual ~GameObject();

	virtual GameObjectTypes Type() = 0;
	virtual std::string TypeName() = 0;
	bool IsCollidingWith(GameObject* other_object);

	virtual bool CanCollideWith([[maybe_unused]] GameObjectTypes other_object_type);
	virtual void ResolveCollision([[maybe_unused]] GameObject* other_object) {};

	void Destroy();
	bool Destroyed();
	virtual void Update(float dt);
	virtual void Draw(const mat3& world_to_ndc);
	void decide_matrix(const mat3& world_to_ndc);

	const mat3& GetMatrix();
	const mat3& GetFinalMatrix();
	const vec2& GetPosition() const;
	const vec2& GetVelocity() const;
	const vec2& GetScale() const;
	const float GetRotation() const;
	const vec2& GetCollisionScale() const;
	const Engine::ShaderName GetShaderName() const;
	const vec4& Getcolor() const;


	template<typename T>
	T* GetGOComponent() {
		return componentmanagers.GetComponent<T>();
	}
	//
	class State {
	public:
		virtual void Enter(GameObject* object) = 0;
		virtual void Update(GameObject* object, float dt) = 0;
		virtual void CheckExit(GameObject* object) = 0;
		virtual std::string GetName() = 0;
	};
	State* current_state;
	void change_state(State* new_state);
	void SetPosition(vec2 new_position);
	void SetTranslation_Pivot(vec2 value);
	void UpdatePosition(vec2 delta);

	void SetVelocity(vec2 new_position);
	void UpdateVelocity(vec2 delta);

	void SetScale(vec2 new_scale);
	void UpdateScale(vec2 delta);

	void SetRotation(float new_rotation);
	void SetRotation_Pivot(vec2 value);
	void UpdateRotation(float delta);

	void SetCollisionScale(vec2 Collision_scale, bool is_chage);
protected:

	void SetShaderName(Engine::ShaderName input);

	void Setcolor(vec4 input);

	void AddGOComponent(Component* component) {
		componentmanagers.AddComponent(component);
	}
	template<typename T>
	void RemoveGOComponent() {
		componentmanagers.RemoveComponent<T>();
	}
	void ClearGOComponents() {
		componentmanagers.Clear();
	}
	void UpdateGOComponents(float dt) {
		componentmanagers.UpdateAll(dt);
	}

private:
	vec2        scaling{ 1.0f };
	float       angle_disp = 0;
	vec2        position{};
	mat3        final_matrix{};
	mat3        model_to_ndc{};
	mat3        model_to_ndc_for_collision{};
	vec2		collision_scaling{ 1.0f };
	bool		different_collision = false;
	vec4		color{};
	vec2		rotation_pivot{};
	vec2		translation_pivot{};
	vec2		first_setting{ 1280, 720 };

	ComponentManager componentmanagers;
	bool destroy = false;
	vec2 velocity;

	Engine::ShaderName shadername = Engine::ShaderName::basic;

	class State_None : public State {
	public:
		void Enter(GameObject*) override {}
		void Update(GameObject*, float) override {}
		void CheckExit(GameObject*) override {}
		std::string GetName() { return ""; }
	};
	State_None state_none;
};
