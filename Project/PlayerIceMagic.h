//============================================================
// @brief �v���C���[�X���@�N���X
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerIceMagic :public PlayerMagic
{
public:
	PlayerIceMagic();//�R���X�g���N�^
	~PlayerIceMagic();//�f�X�g���N�^

	void Initialize()override;//������
	void Update()override;//�X�V
	const void Draw()const override;//�X�V
private:
	VECTOR firstPosition;//�ŏ��̍��W
	float velocity;
	int frameCount;
	bool isBooting;
};

