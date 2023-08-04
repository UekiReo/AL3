#pragma once
#include "Matrix.h"
#include "EnemyBullet.h"
#include "EnemyState.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TimedCall.h"
#include "CMath.h"

class Player;

class EnemyState;

class Enemy
{
public:
	Enemy();

	~Enemy();

	void Initialize(Model* model, Vector3 pos);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(EnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

	void Fire();

	void FireandReset();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	
	static const int kFireInterval = 60;

	int32_t FireTimer_ = 0;

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasetable_[])();

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Player* player_ = nullptr;

	std::list<EnemyBullet*> bullets_;

	EnemyState* phase_ = nullptr;

	std::list<TimedCall*> timedCalls_;
};