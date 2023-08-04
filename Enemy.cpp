#include "Enemy.h"
#include "Matrix.h"
#include "Player.h"
#include <cassert>
#include "GameScene.h"

Enemy::Enemy() {}

Enemy::~Enemy() 
{
	for (TimedCall* timedCall : timedCalls_)
	{
		delete timedCall;
	}
}

void Enemy::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	// フェーズ開始
	phase_ = new EnemyApproach();

	worldTransform_.Initialize();

	velocity_ = velocity;

	worldTransform_.translation_ = pos;

	FireTimer_ = kFireInterval;
	FireandReset();
}

void Enemy::Update() 
{
	 phase_->Update(this);

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
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision() 
{
	isDead_ = true; 
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
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