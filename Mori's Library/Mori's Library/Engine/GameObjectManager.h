

#pragma once
#include <vector>
#include "GameObject.h"
#include "Matrix.h"

namespace Math { class TransformationMatrix; }

class GameObjectManager : public Component {
public:
	void Add(GameObject* object);
	void Remove(GameObject* object);
	void Unload();

	void UpdateAll(float dt, mat3 world_to_ndc);
	void DrawAll(const mat3& world_to_ndc);

	void CollisionTest();

	template<typename T>
	T* GetObject() {
		for (GameObject* object : objects) {
			T* ptr = dynamic_cast<T*>(object);
			if (ptr != nullptr) {
				return ptr;
			}
		}
		return nullptr;
	}
private:
	std::list<GameObject*> objects;
};
