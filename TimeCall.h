#pragma once

#include <functional>

/// <summary>
/// 時限発動
/// </summary>

class TimeCall
{
public:
	// コンストラクタ
	TimeCall(std::function<void()> callBack, uint32_t time);

	// 更新
	void Update();

	// 完了ならtrueを返す
	bool IsFinished() { return isFinish; }

private:
	// コールバック
	std::function<void()> callBack_;

	// 残り時間
	uint32_t time_;

	// 完了フラグ
	bool isFinish = false;
};
