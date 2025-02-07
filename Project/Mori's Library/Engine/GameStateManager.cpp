

#include "GameStateManager.h"
#include "Engine.h"
#include <iostream>
#include "GameObjectManager.h"

GameStateManager::GameStateManager() { }

void GameStateManager::AddGameState(GameState& gamestate) {
    gamestates.push_back(&gamestate);
}

void GameStateManager::SetNextGameState(int index) {
    last_stage = current_gamestate->GetName();
    next_gamestate = gamestates[index];
}

void GameStateManager::ReloadGameState() {
    status = Status::UNLOADING;
}

void GameStateManager::ClearNextGameState() {
    next_gamestate = nullptr;
}

bool GameStateManager::HasGameEnded() {
    return status == Status::EXIT;
}

std::string GameStateManager::Get_current_state_name()
{
    return current_gamestate->GetName();
}

std::string GameStateManager::Get_last_state_name()
{
    return last_stage;
}

void GameStateManager::Update([[maybe_unused]] float dt) {
    switch (status) {
    case Status::STARTING:
        if (gamestates.size() > 0) {
            next_gamestate = gamestates[0];
            status = Status::LOADING;
        }
        else {
            status = Status::STOPPING;
        }
        break;
    case Status::LOADING:
        current_gamestate = next_gamestate;
        Engine::GetLogger().LogEvent("Load " + current_gamestate->GetName());
        current_gamestate->Load();
        Engine::GetLogger().LogEvent("Load Complete");
        status = Status::UPDATING;
        break;
    case Status::UPDATING:
        if (current_gamestate != next_gamestate) {
            ReloadGameState();
        }
        else {
            //time += dt;
            Engine::GetLogger().LogVerbose("Update " + current_gamestate->GetName());
            current_gamestate->Update(dt);
            current_gamestate->GetGSComponent<GameObjectManager>()->CollisionTest();
            current_gamestate->Draw();

        }
        break;
    case Status::UNLOADING:
        Engine::GetLogger().LogEvent("Unload " + current_gamestate->GetName());
        current_gamestate->Unload();
        Engine::GetLogger().LogEvent("Unload Complete");
        Engine::GetTextureManager().Unload();

        if (next_gamestate == nullptr) { status = Status::STOPPING; }
        else { status = Status::LOADING; }
        break;
    case Status::STOPPING:
        status = Status::EXIT;
        break;
    case Status::EXIT:
        break;
    }
}
