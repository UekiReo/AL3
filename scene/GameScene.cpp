#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include<iostream>
#include<fstream>
#include<string>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_;
	delete player_;
	delete debugCamera_;

	for (Enemy* enemy : enemys_) 
	{
		delete enemy;
	}

	for (EnemyBullet* bullet : enemyBullets_) 
	{
		delete bullet;
	}

	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;
}

void GameScene::Initialize() 
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	input_ = Input::GetInstance();

	// 軸方向表示の表示を有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	// 参照するビュープロジェクションを指定
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition(0, 0, 30);
	player_->Initialize(model_, textureHandle_, playerPosition);

	// 天球
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	player_->Setparent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	UpdateEnemyPopCommands();

	// 敵キャラの更新
	for (Enemy* enemy : enemys_) 
	{
		enemy->Update();
	}

	// 敵弾の更新
	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Update();
	}

	// デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) 
		{
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet)
	{
		if (bullet->IsDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});

	CheckAllCollisions();

	skydome_->Update();

	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN))
	{
		if (isDebugCameraActive_ == false) 
		{
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif
	// カメラの処理
	if (isDebugCameraActive_)
	{
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
}

void GameScene::Draw() 
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	for (Enemy* enemy : enemys_) 
	{
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) 
	{
		bullet->Draw(viewProjection_);
	}

	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

/// 敵発生データの読み込み
void GameScene::LoadEnemyPopData() 
{
	// ファイルを開く
	std::ifstream file;
	file.open("csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

/// 敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands() {
	if (isWaitTime_ == true) {
		waitTimer_--;
		if (waitTimer_-- <= 0) {
			isWaitTime_ = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			SpawnEnemy({x, y, z}, {0.0f, 0.0f, -0.5f});

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());
			isWaitTime_ = true;
			waitTimer_ = waitTime;

			break;
		}
	}
}

void GameScene::SpawnEnemy(Vector3 pos, Vector3 velocity) 
{
	// 敵を発生させる
	Enemy* enemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	// 敵の初期化
	enemy->Initialize(model_, pos, velocity);

	// リストに登録
	enemys_.push_back(enemy);
}

void GameScene::CheckAllCollisions()
{
	// 半径
	const float radius = 2.0f;

	Vector3 posA, posB;
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_)
	{
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		// 球同士が当たっていれば
		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
		    (radius * radius))
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標

	// 自弾と敵キャラの全ての当たり判定
	for (Enemy* enemy : enemys_) 
	{
		for (PlayerBullet* bullet : playerBullets) 
		{
			// 自弾の座標
			posA = enemy->GetWorldPosition();
			posB = bullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

			// 球同士が当たっていれば
			if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
			    (radius * radius))
			{
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets) 
	{
		for (EnemyBullet* enemyBullet : enemyBullets_) 
		{
			// 自弾の座標
			posA = playerBullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

			// 球同士が当たっていれば
			if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
			    (radius * radius)) 
			{
				// 敵キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
