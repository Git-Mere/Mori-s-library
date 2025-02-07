#pragma once
#include "Skill.h"
#include "Component.h"

class SkillManager : public Component {
public:
	~SkillManager() { Clear(); }
	void Update(float dt) {
		for (Skill* component : components) {
			component->Cooling(dt);
			if (component->use()) {
				component->func();
			}
		}
	}
	void AddComponent(Skill* component) {
		components.push_back(component);
	}
	template<typename T>
	T* GetComponent() {
		for (Skill* component : components) {
			T* ptr = dynamic_cast<T*>(component);
			if (ptr != nullptr) {
				return ptr;
			}
		}
		return nullptr;
	}
	template<typename T>
	void RemoveComponent() {
		auto it = std::find_if(
			components.begin(), components.end(), [](Skill* element) {
				return (dynamic_cast<T*>(element) != nullptr);
			}
		);
		delete* it;
		components.erase(it);
	}
	void Clear() {
		for (Skill* component : components) {
			delete component;
		}
		components.clear();
	}
	Skill* Get_Skill(int i) {
		if (i >= components.size()) {
			return nullptr;
		}
		return components[i];
	}

	size_t Get_size() {
		return components.size();
	}

	bool check_special_ani() {
		for (Skill* component : components) {
			if (component->now_delay() == true) {
				return false;
			}
		}
		return true;
	}
private:
	std::vector<Skill*> components;
};