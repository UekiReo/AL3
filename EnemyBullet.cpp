#include "EnemyBullet.h"
#include "Matrix.h"
#include <cassert>
#include "CMath.h"
#include "Player.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) 
{
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	velocity_ = velocity;
}

void EnemyBullet::Update() 
{
	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, velocity_);

	// 時間経過で消滅
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::OnCollision()
{ 
	isDead_ = true;
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}