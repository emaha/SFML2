#include "Buff.h"


Buff::Buff(){}
Buff::~Buff(){}

Buff::Buff(BuffType buffType, float buffTime, float amount)
{
	this->amount = amount;
	this->buffTime = buffTime;
	buffTick = 0.1f;
}

void Buff::update(float time)
{
	isActive = buffTime >= .0f;
	if (isActive)
	{
		if (buffTick<=0.0f)
		{
			

			buffTick = 0.1f;
		}
		

		buffTime -= time;
		buffTick -= time;
	}
}

bool Buff::isReady()
{
	if (buffTime > .0f && buffTick<0.0f)
	{
		return true;
	}
	return false;
}



