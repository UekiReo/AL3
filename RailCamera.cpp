#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& pos, const Vector3& rotation) 
{
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();
}

void RailCamera::Update() 
{
	worldTransform_.translation_.z += 0.001f;
	worldTransform_.rotation_.y += 0.0003f;
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("RailCamera");
	ImGui::DragFloat3("CameraTranslation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}