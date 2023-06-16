#include "TimeCall.h"

TimeCall::TimeCall(std::function<void()> callBack, uint32_t time) 
{
	this->callBack_ = callBack;
	this->time_ = time;
}

void TimeCall::Update() 
{
	if (isFinish) 
	{
		return;
	}

	time_--;

	if (time_ <= 0) 
	{
		isFinish = true;

		// コールバック関数の呼び出し
		callBack_();
	}
}
