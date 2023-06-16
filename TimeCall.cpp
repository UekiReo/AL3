#include "TimeCall.h"

TimeCall::TimeCall(std::function<void()> callback, uint32_t time) 
{ 
	this->callBack_ = callback;
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

		// コールバック関数呼び出し
		callBack_();
	}
}
