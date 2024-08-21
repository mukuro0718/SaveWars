//===========================================================
// @brief �v���C���[���@�N���X
//===========================================================
#pragma once

class PlayerMagic abstract
{
public:
	PlayerMagic();//�R���X�g���N�^
	virtual ~PlayerMagic(){}//�f�X�g���N�^

	virtual		  void Initialize()		 abstract;
	virtual		  void Update	 ()		 abstract;
	virtual const void Draw		 ()const abstract;//�`��

	/*setter*/
	void OnIsStart() { this->isStart = true; }
protected:
	/*�����o�ϐ�*/
	VECTOR position;	//���W
	VECTOR moveVector;	//�ړ��x�N�g��
	float  radius;		//���a
	bool   isInitialize;//�������t���O
	bool isStart;
};

