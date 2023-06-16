#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "EnemyBullet.h"
#include "TimedCall.h"
#include <List>

class Enemy 
{
public:
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Fire();

	/// <summary>
	/// 弾を発射し、タイマーをリセットする関数
	/// </summary>
	void FireAndReset();

	void Draw(ViewProjection viewProjection);

	void ApproachPhaseInitialize();

	void ApproachPhaseUpdate();

	void LeavePhaseUpdate();

	enum class Phase {
		Start,
		Approach,
		Leave,
	};

	static const int kFireInterval = 60;

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	Phase phase_ = Phase::Start;

	std::list<EnemyBullet*> bullets_;

	// 時限発動のリスト
	std::list<TimedCall*> timedCall_;

	int32_t shotTimer_ = 0;
};
