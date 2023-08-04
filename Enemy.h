#pragma once
#include "Matrix.h"
#include "EnemyBullet.h"
#include "EnemyState.h"
#include "Model.h"
#include "WorldTransform.h"
#include "TimedCall.h"
#include "CMath.h"
#include <list>

class Player;
class EnemyState;
class GameScene;

class Enemy
{
public:
	Enemy();

	~Enemy();

	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(EnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

	void Fire();

	void FireandReset();

	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }
	
	bool IsDead() const { return isDead_; };

public:
	static const int kFireInterval = 60;

	int32_t FireTimer_ = 0;

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasetable_[])();

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Player* player_ = nullptr;

	//std::list<EnemyBullet*> bullets_;

	EnemyState* phase_ = nullptr;

	Vector3 velocity_;

	std::list<TimedCall*> timedCalls_;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	bool isDead_ = false;
};