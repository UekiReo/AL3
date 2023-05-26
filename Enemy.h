#pragma once
#include "Model.h"
#include "WorldTransform.h"

enum class Phase 
{
	Approach,
	Leave,
};

class Enemy 
{
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// 自作メンバ関数
	void Approach();

	void Leave();

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phaseTable_[])();

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Vector3 move_;

	float kApproachSpeed_;

	float kLeaveSpeed_;

	Phase phase_ = Phase::Approach;
};