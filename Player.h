﻿#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

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

	Model* model_ = nullptr;
};
