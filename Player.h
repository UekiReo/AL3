#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"
#include "CollisionConfig.h"

class Player: public Collider
{
public:
	Player();

	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition() override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;
};
