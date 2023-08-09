#include "Enemy.h"
#include "Matrix.h"
#include "Player.h"
#include <cassert>

Enemy::Enemy() {}

Enemy::~Enemy()
{
	for (EnemyBullet* bullet : bullets_) 
	{
		delete bullet;
	}

	for (TimedCall* timedCall : timedCalls_)
	{
		delete timedCall;
	}
}

void Enemy::Initialize(Model* model)
{
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	// フェーズ開始
	/* phase_ = new EnemyApproach;*/

	worldTransform_.Initialize();

	worldTransform_.translation_ = {10, 0, 20};

	FireTimer_ = kFireInterval;
	FireandReset();

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeEnemy);
}

void Enemy::Update()
{
	// デスフラグの立った弾の削除
	bullets_.remove_if([](EnemyBullet* bullet) 
	{
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

	/* phase_->Update(this);*/

	// タイマー
	timedCalls_.remove_if([](TimedCall* timedcall)
	{
		if (timedcall->IsFinish())
		{
			delete timedcall;
			return true;
		}
		return false;
	});

	for (TimedCall* timedCall : timedCalls_) 
	{
		timedCall->Update();
	}

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// 弾更新
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Update();
	}
}

void Enemy::ChangePhase(EnemyState* newState) 
{
	delete phase_;
	phase_ = newState;
}

void Enemy::Move(Vector3 speed) 
{
	worldTransform_.translation_ += speed; 
};

void Enemy::Fire() 
{
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = -1.0f;

	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = this->GetWorldPosition();
	Vector3 velocity = Subtract(playerPosition, enemyPosition);
	velocity = Normalise(velocity);
	velocity.x *= -kBulletSpeed;
	velocity.y *= -kBulletSpeed;
	velocity.z *= -kBulletSpeed;

	// 弾を生成し初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->SetPlayer(player_);
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::OnCollision() {}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::FireandReset()
{
	// 攻撃処理
	Fire();

	// タイマー
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireandReset, this), kFireInterval));
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}