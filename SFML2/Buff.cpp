#include "Buff.h"


Buff::Buff(){}
Buff::~Buff(){}

Buff::Buff(BuffType buffType, float buffTime, float amount)
{
	this->buffType = buffType;
	this->amount = amount;
	this->buffTime = buffTime;
	buffTick = 100;
}

void Buff::update(float time)
{
	isActive = buffTime >= 0;
	if (isActive)
	{
		if (buffTick<=0)
		{
			

			buffTick = 100;
		}
		

		buffTime -= time;
		buffTick -= time;
	}
}

bool Buff::isReady()
{
	if (buffTime > 0 && buffTick<0)
	{
		return true;
	}
	return false;
}



