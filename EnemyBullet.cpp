#include "EnemyBullet.h"
#include "Matrix.h"
#include <cassert>

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

	// Y軸角度
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velocityXZ = Length({velocity_.x, 0.0f, velocity_.z});

	// X軸角度
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);
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

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}