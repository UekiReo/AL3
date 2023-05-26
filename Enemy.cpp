#include "Enemy.h"
#include "Matrix.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) 
{
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	

	move_ = {0, 0.005f, 0};

	kApproachSpeed_ = 0.003f;

	kLeaveSpeed_ = 0.005f;

	static_cast<size_t>(phase_);
}

void (Enemy::*Enemy::phaseTable_[])() = 
{
    &Enemy::Approach, // 接近
    &Enemy::Leave     // 離脱
};

// 接近
void Enemy::Approach() 
{
	move_.z -= kApproachSpeed_;
	if (worldTransform_.translation_.z < 0.0f) 
	{
		(this->*phaseTable_[1])();
	}
}

// 離脱
void Enemy::Leave() 
{
	move_.x -= kLeaveSpeed_;
	move_.y += kLeaveSpeed_;
}

void Enemy::Update() 
{
	//Vector3 move = {0, 0, 0};
	//const float kCharacterSpeed = 0.2f;

	//// フェーズと移動
	//switch (phase_)
	//{ 
	//// 接近フェーズ
	//case Phase::Approach:
	//default:
	//	move.z -= kCharacterSpeed;
	//	if (worldTransform_.translation_.z < 0.0f) 
	//	{
	//		phase_ = Phase::Leave;
	//	}
	//	break;
	//// 離脱フェーズ
	//case Phase::Leave:
	//	move.x -= kCharacterSpeed;
	//	move.y += kCharacterSpeed;
	//	break;
	//}

	(this->*phaseTable_[0])();

	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, move_);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}