#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {}

void Skydome::Draw(const ViewProjection& viewPrijection) 
{
	model_->Draw(worldTransform_, viewPrijection);
}
