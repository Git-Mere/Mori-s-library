

#pragma once
#include <vector>
#include <SDL2/SDL_events.h>

class Input {
public:
	enum class Keys {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N,
		O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Space, Enter, Left, Up, Right, Down, ESC, Tilde,
		Unknown
	};

	Input();
	void Update();
	void SetKeyDown(Keys key, bool value);
	bool KeyDown(Keys key);
	bool KeyJustPressed(Keys key);
	bool KeyJustReleased(Keys key);
	void on_key_pressed(const SDL_Event& event);
	void on_key_released(const SDL_Event & event);
	bool PressAnyKey();
private:
	std::vector<bool> keys_down;
	std::vector<bool> previous_keys_down;
};