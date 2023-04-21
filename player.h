#include "GameScene.h"
#include "WorldTransform.h"
#pragma once

/// <summary>
/// 自キャラ
/// </summary>

class Player 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// ワールド変換データ
	WorldTransform woridtransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

};