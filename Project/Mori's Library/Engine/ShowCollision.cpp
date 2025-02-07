

#include "ShowCollision.h"
#include "Engine.h"

ShowCollision::ShowCollision() : enabled(false) {}

void ShowCollision::Update([[maybe_unused]] float dt) {
#ifdef _DEBUG				
    if (Engine::GetInput().KeyJustReleased(Input::Keys::Tilde) == true) {
        enabled = !enabled;
    }
#endif
}

bool ShowCollision::Enabled() {
    return enabled;
}
