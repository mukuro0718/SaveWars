//========================================
// @brief �e�N���X
// �v���C���[�ƃG�l�~�[�ɑ΂��Ċۉe�̕`����s��
//========================================
#pragma once
#include "Singleton.h"

class Shadow : public Singleton<Shadow>
{
public:
	friend class Singleton<Shadow>;

	void Final();//�ŏI����
	void Draw(const int _stageModelHandle, const VECTOR _targetPos, const float _shadowHeight, const float _shadowSize);//�e��`��
private:
	/*���������֐�*/
			 Shadow();	//�R���X�g���N�^
	virtual ~Shadow();	//�f�X�g���N�^

	/*�����o�ϐ�*/
	int imageHandle;//�e�摜�n���h��
};

