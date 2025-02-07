#pragma once
#include "Component.h"

class Gravity : public Component {
public:
	Gravity(float gravity_value) : gravity_value(gravity_value) {};
	float GetValue() { return gravity_value; }
private:
	float gravity_value;
};
