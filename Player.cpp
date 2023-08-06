#include "Player.h"
#include "Matrix.h"
#include "ImGuiManager.h"
#include <cassert>

Player::Player() {}

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet;
	}

	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	fireTimer = 30;
	worldTransform_.translation_ = Add(worldTransform_.translation_, pos);

	// 3Dレティクルのワールドトランスフォームの初期化
	worldtransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("reticle.png");

	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection viewProjection) 
{
	// デスフラグの立った弾の削除
	bullets_.remove_if([](PlayerBullet* bullet) 
		{
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) 
	{
		move.x -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) 
	{
		move.y += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}

	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate))
	{
		move.x += (float)joystate.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joystate.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 旋回処理
	Rotate();

	// 攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Update();
	}

	// Guiによる移動
	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	ImGui::Begin("Player");
	ImGui::Text("debugCamera = Enter");
	ImGui::SliderFloat3("Player", inputFloat3, -40.0f, 40.0f);
	ImGui::End();

	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];

	worldTransform_.translation_ = VectorAdd(worldTransform_.translation_, move);

	worldTransform_.UpdateMatrix();

	SetReticle(viewProjection);

	//// 自機から3Dレティクルへの距離
	//const float kDistancePlayerTo3DReticle = 50.0f;
	//// 自機から3Dレティクルのオフセット(Z+向き)
	//Vector3 offset = {0, 0, 1.0f};

	//// 自機のワールド行列の回転を反映
	//offset = TransformNormal(offset, worldTransform_.matWorld_);
	//// ベクトルの長さを整える
	//offset = Multiply(kDistancePlayerTo3DReticle, Normalise(offset));

	//// 3Dレティクルの座標を設定
	//worldtransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	//worldtransform3DReticle_.UpdateMatrix();

	//Vector3 positionReticle = {
	//    worldtransform3DReticle_.matWorld_.m[3][0], worldtransform3DReticle_.matWorld_.m[3][1],
	//    worldtransform3DReticle_.matWorld_.m[3][2]};

	//Matrix4x4 matViewport =
	//    MakeViewportMatrix(0.0f, 0.0f, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);

	//Matrix4x4 matViewProjectionViewport =
	//    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Rotate()
{
	// 回転速さ
	const float kRotSpeed = 0.02f;

	// 推した方向で移動ベクトル変更
	if (input_->PushKey(DIK_A)) 
	{
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

void Player::Attack()
{
	fireTimer--;

	if (fireTimer <= 0)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機に合わせて回転させる
			velocity = Subtract(
			    {worldtransform3DReticle_.matWorld_.m[3][0],
			     worldtransform3DReticle_.matWorld_.m[3][1],
			     worldtransform3DReticle_.matWorld_.m[3][2]},
			    GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalise(velocity));

			// 弾を生成し初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
			fireTimer = 30;
		}

		XINPUT_STATE joystate;
		if (!Input::GetInstance()->GetJoystickState(0, joystate))
		{
			return;
		}

		if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
		{
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機に合わせて回転させる
			velocity = Subtract(
			    {worldtransform3DReticle_.matWorld_.m[3][0],
			     worldtransform3DReticle_.matWorld_.m[3][1],
			     worldtransform3DReticle_.matWorld_.m[3][2]},
			    GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalise(velocity));

			// 弾を生成し初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
			fireTimer = 30;
		}
	}
}

void Player::OnCollision() {}

void Player::Draw(ViewProjection viewProjection)
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI()
{ 
	sprite2DReticle_->Draw(); 
}


Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::Setparent(const WorldTransform* parent)
{ 
	worldTransform_.parent_ = parent; 
}

void Player::SetReticle(const ViewProjection viewProjection) 
{
	const float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset = {0, 0, 1.0f};

	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Multiply(kDistancePlayerTo3DReticle, Normalise(offset));

	worldtransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	worldtransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle = {
	    worldtransform3DReticle_.matWorld_.m[3][0], worldtransform3DReticle_.matWorld_.m[3][1],
	    worldtransform3DReticle_.matWorld_.m[3][2]};

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0.0f, 0.0f, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);

	Matrix4x4 matViewProjectionViewport =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	/* sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));*/

	// マウスの場合
	/* POINT mousePosition;*/

	// マウス座標（スクリーン座標）を取得する
	/* GetCursorPos(&mousePosition);*/

	// クライアントエリア座標に変換する
	/* HWND hwnd = WinApp::GetInstance()->GetHwnd();
	 ScreenToClient(hwnd, &mousePosition);
	 sprite2DReticle_->SetPosition({(float)mousePosition.x, (float)mousePosition.y});*/

	// コントローラーの場合
	XINPUT_STATE joystate;

	Vector2 spritePostition = sprite2DReticle_->GetPosition();
	if (Input::GetInstance()->GetJoystickState(0, joystate)) 
	{
		spritePostition.x += (float)joystate.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePostition.y -= (float)joystate.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePostition);
	}

	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 posNear = {(float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 0};

	Vector3 posFar = {(float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 1};
	
	posNear = Transform(posNear, matInverseVPV);
	
	posFar = Transform(posFar, matInverseVPV);
	
	Vector3 mouseDirection = Subtract(posFar, posNear);
	
	mouseDirection = Normalise(mouseDirection);
	
	const float kDistancetestObject = 100.0f;
	
	worldtransform3DReticle_.translation_ = Add(posNear, Multiply(kDistancetestObject, mouseDirection));
	
	worldtransform3DReticle_.UpdateMatrix();
	
	ImGui::Begin("Player");
	ImGui::Text("Near:(%+.2f,%+.2f,%.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DRetixle:(%+.2f,%+.2f,%.2f)", worldtransform3DReticle_.translation_.x, worldtransform3DReticle_.translation_.y, worldtransform3DReticle_.translation_.z);
	ImGui::End();
}