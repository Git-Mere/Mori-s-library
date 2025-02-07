

#pragma once
#include "Component.h"

class ShowCollision : public Component {
public:
	ShowCollision();
	void Update([[maybe_unused]] float dt) override;
	bool Enabled();
private:
	bool enabled;
};
