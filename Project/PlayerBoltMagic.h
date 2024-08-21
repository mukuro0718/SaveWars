//============================================================
// @brief �v���C���[�����@�N���X
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerBoltMagic:public PlayerMagic
{
public:
	PlayerBoltMagic();//�R���X�g���N�^
	~PlayerBoltMagic();//�f�X�g���N�^

	void Initialize	()override;//������
	void Update		()override;//�X�V
	const void Draw	()const override;//�X�V
private:
	VECTOR endPosition;//�ŏ��̍��W
	float velocity;
	int frameCount;
	bool isBooting;//���@���N��������
};

