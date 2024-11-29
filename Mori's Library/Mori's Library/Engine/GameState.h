

#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include <string>

class GameState {
public:
	virtual void Load() = 0;
	virtual void Update([[maybe_unused]] float dt) = 0;
	virtual void Unload() = 0;
	virtual void Draw() = 0;
	virtual std::string GetName() = 0;
	virtual void check_clear([[maybe_unused]] float dt) {};
	virtual void end_effect() {};
	template<typename T>
	T* GetGSComponent() {
		return componentmanager.GetComponent<T>();
	}
protected:
	void AddGSComponent(Component* component) {
		componentmanager.AddComponent(component);
	}
	void UpdateGSComponents(float dt) {
		componentmanager.UpdateAll(dt);
	}
	template<typename T>
	void RemoveGSComponent() {
		componentmanager.RemoveComponent<T>();
	}
	void ClearGSComponents() {
		componentmanager.Clear();
	}
	bool clear = false;
	bool clear_effect = true;
private:
	ComponentManager componentmanager;

};

