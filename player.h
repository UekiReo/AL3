#include "GameScene.h"
#include "WorldTransform.h"
#pragma once

/// <summary>
/// ���L����
/// </summary>

class Player 
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform woridtransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

};