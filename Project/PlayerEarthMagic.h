//============================================================
// @brief �v���C���[�y���@�N���X
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerEarthMagic :public PlayerMagic
{
public:
	PlayerEarthMagic();//�R���X�g���N�^
	~PlayerEarthMagic();//�f�X�g���N�^

	void Initialize()override;//������
	void Update()override;//�X�V
	const void Draw()const override;//�X�V
private:
	int frameCount;
};

