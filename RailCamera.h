#pragma once
#include "ImGuiManager.h"
#include "Matrix.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera
{
public:
	// 初期化
	void Initialize(const Vector3& pos, const Vector3& rotate);
	
	void Update();
	
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
