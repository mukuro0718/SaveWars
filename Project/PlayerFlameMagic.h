//============================================================
// @brief �v���C���[�����@�N���X
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerFlameMagic :public PlayerMagic
{
public:
	PlayerFlameMagic();//�R���X�g���N�^
	~PlayerFlameMagic();//�f�X�g���N�^

	void Initialize()override;//������
	void Update()override;//�X�V
	const void Draw()const override;//�X�V
private:
	VECTOR endPosition;//���@�͈̔͏I�_
	int frameCount;
	float range;
};

