#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
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

	// 敵キャラの生成
	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	// 敵キャラの初期化
	Vector3 enemyPosition = {10, 0, 100};
	enemy_->Initialize(model_, enemyPosition);

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	player_->Setparent(&railCamera_->GetWorldTransform());
}

void GameScene::Update() 
{
	// 自キャラの更新
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

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
	enemy_->Draw(viewProjection_);
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
void GameScene::CheckAllCollisions()
{
	float enemyBulletRadius = 0.5f;
	float playerBulletRadius = 0.5f;
	float playeyrRadius = 1.0f;
	float enemyRadius = 1.0f;

	Vector3 posA, posB;
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets)
	{
		posB = bullet->GetWorldPosition();

		Vector3 Distance = {
		    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z)};

		if (Distance.x + Distance.y + Distance.z <=
		    (playeyrRadius + enemyBulletRadius) * (playeyrRadius + enemyBulletRadius)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラ
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) 
	{
		posB = bullet->GetWorldPosition();

		Vector3 Distance = {
		    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z)};

		if (Distance.x + Distance.y + Distance.z <=
		    (enemyRadius + playerBulletRadius) * (enemyRadius + playerBulletRadius)) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾
	for (EnemyBullet* eBullet : enemyBullets)
	{

		posA = eBullet->GetWorldPosition();
		for (PlayerBullet* pbullet : playerBullets)
		{
			posB = pbullet->GetWorldPosition();

			Vector3 Distance = {
			    (posB.x - posA.x) * (posB.x - posA.x), (posB.y - posA.y) * (posB.y - posA.y),
			    (posB.z - posA.z) * (posB.z - posA.z)};

			if (Distance.x + Distance.y + Distance.z <=
			    (enemyBulletRadius + playerBulletRadius) *
			        (enemyBulletRadius + playerBulletRadius)) {
				eBullet->OnCollision();
				pbullet->OnCollision();
			}
		}
	}
#pragma endregion
}
