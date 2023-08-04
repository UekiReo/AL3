#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "CMath.h"
#include <list>

class Player
{
public:
	Player();

	~Player();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	void Update();

	void Draw(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void Setparent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;
};
