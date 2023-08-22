#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Matrix.h"
#include "Input.h"

class Player 
{
public:
	// 初期化
	void Initialize(Model* model);

	// 更新
	void Update();

	// 描画
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	const ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;
};
