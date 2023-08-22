#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model) 
{ 
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
}

void Player::Update()
{
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	if (Input::GetInstance()->GetJoystickState(0, joyState_)) 
	{
		const float kPlayerSpeed = 0.3f;

		Vector3 move = {
		    (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f,(float)joyState_.Gamepad.sThumbLY / SHRT_MAX};

		move = Multiply(kPlayerSpeed, Normalize(move));

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);

		move = TransformNormal(move, cameraRotateMatrix);

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	ImGui::Begin("Player");
	ImGui::Text("debugCamera = Enter");
	/*ImGui::SliderFloat3("Player", inputFloat3, -40.0f, 40.0f);*/
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}
