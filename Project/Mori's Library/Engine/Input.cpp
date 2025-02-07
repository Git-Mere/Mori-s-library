#include "Input.h"
#include "Engine.h"

Input::Input() {
    keys_down.resize(static_cast<int>(Keys::Unknown));
    previous_keys_down.resize(static_cast<int>(Keys::Unknown));
}

void Input::Update() {
    previous_keys_down = keys_down;
}

void Input::SetKeyDown(Keys key, bool value) {
    keys_down[static_cast<int>(key)] = value;
}

Input::Keys convert_opengl(const SDL_Event& event) {
    switch (event.key.keysym.sym) {
    case SDLK_a:
        return Input::Keys::A;
    case SDLK_b:
        return Input::Keys::B;
    case SDLK_c:
        return Input::Keys::C;
    case SDLK_d:
        return Input::Keys::D;
    case SDLK_e:
        return Input::Keys::E;
    case SDLK_f:
        return Input::Keys::F;
    case SDLK_g:
        return Input::Keys::G;
    case SDLK_h:
        return Input::Keys::H;
    case SDLK_i:
        return Input::Keys::I;
    case SDLK_j:
        return Input::Keys::J;
    case SDLK_k:
        return Input::Keys::K;
    case SDLK_l:
        return Input::Keys::L;
    case SDLK_m:
        return Input::Keys::M;
    case SDLK_n:
        return Input::Keys::N;
    case SDLK_o:
        return Input::Keys::O;
    case SDLK_p:
        return Input::Keys::P;
    case SDLK_q:
        return Input::Keys::Q;
    case SDLK_r:
        return Input::Keys::R;
    case SDLK_s:
        return Input::Keys::S;
    case SDLK_t:
        return Input::Keys::T;
    case SDLK_u:
        return Input::Keys::U;
    case SDLK_v:
        return Input::Keys::V;
    case SDLK_w:
        return Input::Keys::W;
    case SDLK_x:
        return Input::Keys::X;
    case SDLK_y:
        return Input::Keys::Y;
    case SDLK_z:
        return Input::Keys::Z;
    case SDLK_SPACE:
        return Input::Keys::Space;
    case SDLK_RETURN:
        return Input::Keys::Enter;
    case SDLK_LEFT:
        return Input::Keys::Left;
    case SDLK_UP:
        return Input::Keys::Up;
    case SDLK_RIGHT:
        return Input::Keys::Right;
    case SDLK_DOWN:
        return Input::Keys::Down;
    case SDLK_ESCAPE:
        return Input::Keys::ESC;
    case SDLK_BACKQUOTE:
        return Input::Keys::Tilde;
    }

    return Input::Keys::Unknown;
}

void Input::on_key_pressed(const SDL_Event& event) {
    Input::Keys key = convert_opengl(event);
    if (key != Input::Keys::Unknown) {
        Engine::GetLogger().LogDebug("Key Pressed");
        Engine::GetInput().SetKeyDown(key, true);
    }
}

void Input::on_key_released(const SDL_Event& event) {
    Input::Keys key = convert_opengl(event);
    if (key != Input::Keys::Unknown) {
        Engine::GetLogger().LogDebug("Key Released");
        Engine::GetInput().SetKeyDown(key, false);
    }
}

bool Input::KeyDown(Keys key) {
    return keys_down[static_cast<int>(key)];
}

bool Input::KeyJustPressed(Keys key) {
    return keys_down[static_cast<int>(key)] == true && previous_keys_down[static_cast<int>(key)] == false;
}

bool Input::KeyJustReleased(Keys key) {
    return keys_down[static_cast<int>(key)] == false && previous_keys_down[static_cast<int>(key)] == true;
}

bool Input::PressAnyKey() {
    for (bool key_state : keys_down) {
        if (key_state) {
            return true;
        }
    }
    return false;
}