#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyState.h"
#include "Matrix.h"

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

private:
	static void (Enemy::*phasetable_[])();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	EnemyState* phase_ = nullptr;
};