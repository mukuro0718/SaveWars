//============================================================
// @brief �v���C���[�����@�N���X
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerStormMagic :public PlayerMagic
{
public:
	PlayerStormMagic();//�R���X�g���N�^
	~PlayerStormMagic();//�f�X�g���N�^

	void Initialize()override;//������
	void Update()override;//�X�V
	const void Draw()const override;//�X�V
private:
	VECTOR endPosition;//�ŏ��̍��W
	float velocity;
	int frameCount;
};

