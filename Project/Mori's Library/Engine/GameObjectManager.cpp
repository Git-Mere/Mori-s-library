

#include "GameObjectManager.h"
#include "Engine.h"
#include <iostream>
#include "../Mori/Player.h"

void GameObjectManager::Add(GameObject* object)
{
	objects.push_back(object);
}

void GameObjectManager::Remove(GameObject* object)
{
	objects.remove(object);
}

void GameObjectManager::Unload()
{
	for (auto object : objects) {
		delete object;
	}
	objects.clear();
}

void GameObjectManager::UpdateAll(float dt, mat3 world_to_ndc)
{
	std::vector<GameObject*> destroy_objects;
	for (GameObject* a : objects) {
		a->Update(dt);
		a->decide_matrix(world_to_ndc);
		if (a->Destroyed()) { destroy_objects.push_back(a); }
		
	}
	for (GameObject* a : destroy_objects) {
		objects.remove(a);
		delete a;
	}
}

void GameObjectManager::DrawAll(const mat3& world_to_ndc)
{
	for (GameObject* a : objects) {
		a->Draw(world_to_ndc);
	}
}

void GameObjectManager::CollisionTest() {
	for (auto object_1 : objects) {
		for (auto object_2 : objects) {
			if (object_1 != object_2 && object_1->CanCollideWith(object_2->Type())) {
				if (object_1->IsCollidingWith(object_2)) {
					Engine::GetLogger().LogEvent("Collision Detected: " + object_1->TypeName() + " and " + object_2->TypeName());
					object_1->ResolveCollision(object_2);
				}
			}
		}
	}
}
