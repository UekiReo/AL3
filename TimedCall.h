#pragma once
#include <functional>

class TimedCall 
{
public:
	TimedCall(std::function<void()> callBack, uint32_t time);
	void Update();
	bool IsFinish() { return isFinish; }

private:
	std::function<void()> callBack_;
	uint32_t time_;
	bool isFinish = false;
};