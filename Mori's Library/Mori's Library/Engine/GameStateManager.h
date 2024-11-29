

#pragma once
#include "GameState.h"
#include <vector>

class GameStateManager {
public:
	template<typename T>
	T* GetGSComponent() { return current_gamestate->GetGSComponent<T>(); }
	GameStateManager();

	void Update([[maybe_unused]] float dt);

	void AddGameState(GameState& gamestate);
	void SetNextGameState(int index);
	void ClearNextGameState();
	void ReloadGameState();
	bool HasGameEnded();
	std::string Get_current_state_name();
	std::string Get_last_state_name();
private:
	enum class Status {
		STARTING,
		LOADING,
		UPDATING,
		UNLOADING,
		STOPPING,
		EXIT
	};

	Status status = Status::STARTING;
	std::vector<GameState*> gamestates;
	GameState* current_gamestate = nullptr;
	GameState* next_gamestate = nullptr;
	std::string last_stage = "";
	float time = 0;
};

