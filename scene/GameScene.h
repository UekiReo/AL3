#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "WorldTransform.h"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵の発生
	/// </summary>
	void SpawnEnemy(Vector3 pos, Vector3 velocity);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// <summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// プレイヤー
	Player* player_ = nullptr;

	// デバッグカメラ有効化
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵関連のリスト
	std::list<Enemy*> enemys_;
	std::list<EnemyBullet*> enemyBullets_;

	// 天球
	Skydome* skydome_;
	Model* skydomeModel_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_;

	std::stringstream enemyPopCommands;
	bool isWaitTime_; 
	int32_t waitTimer_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
