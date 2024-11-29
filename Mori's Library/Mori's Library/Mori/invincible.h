#pragma once
#include "../Engine/Component.h"
#include "Timer.h"

class Invincible : public DownTimer {
public:
	Invincible();
	bool is_hurt();
	bool is_invin();
	void Set_invin(float time, bool hurt);
	void Update(float dt) override;
private:
	bool invin_type =  false;
};