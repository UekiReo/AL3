#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "CMath.h"
#include <Sprite.h>
#include <list>

class Player
{
public:
	Player();

	~Player();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	void Update(ViewProjection viewProjection);

	void Draw(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	void DrawUI();

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

	// 3Dレティクルのワールドトランスフォーム
	WorldTransform worldtransform3DReticle_;
	Sprite* sprite2DReticle_ = nullptr;
};
