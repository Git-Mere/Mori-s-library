#include "invincible.h"


Invincible::Invincible() : DownTimer(0)
{
}

bool Invincible::is_hurt()
{
	return invin_type;
}

bool Invincible::is_invin()
{
	return !end();
}

void Invincible::Set_invin(float time, bool hurt)
{
	if (end() == true) {
		Set(time);
	}
	invin_type = hurt;
}

void Invincible::Update(float dt)
{
	DownTimer::Update(dt);
	if (invin_type == true && end()) {
		invin_type = false;
	}
}
