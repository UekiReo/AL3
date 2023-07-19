#pragma once
#include "Matrix.h"
#include "CMath.h"
#include "EnemyBullet.h"
#include "EnemyState.h"
#include "Model.h"
#include "TimedCall.h"
#include "WorldTransform.h"

class EnemyState;

class Enemy
{
public:
	Enemy();

	~Enemy();

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(EnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

	void Fire();

	// 弾を発射しリセットする関数
	void FireandReset();

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasetable_[])();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	std::list<EnemyBullet*> bullets_;

	EnemyState* phase_ = nullptr;

	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;

public:
	static const int kFireInterval = 60;

	int32_t FireTimer_ = 0;
};