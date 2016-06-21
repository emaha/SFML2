#pragma once

#include "Constants.h"

class Buff
{
public:
	Buff();
	~Buff();

	Buff(BuffType buffType, float buffTime, float amount);

	void update(float time);
	bool isReady();


	BuffType buffType;

	float amount;
	float buffTime;
	float buffTick;

	bool isActive;

};

