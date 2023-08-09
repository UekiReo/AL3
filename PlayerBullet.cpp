#include "PlayerBullet.h"
#include "Matrix.h"
#include "CMath.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をセット
	velocity_ = velocity;
}

void PlayerBullet::Update() 
{
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) 
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) 
{
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
