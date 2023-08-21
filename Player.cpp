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
