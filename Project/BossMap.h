//===========================================
// @brief �n�ʃN���X
// �n�ʂ̕`����s��
//===========================================
#pragma once

class ColliderData;
class BossMap
{
public:
	BossMap();//�R���X�g���N�^
	~BossMap();//�f�X�g���N�^

		  void	Initialize		();		//������
		  void	Finalize		();		//�㏈��
		  void	Update			();		//�X�V
	const void	Draw			()const;//�`��

	/*getter*/
	const int	GetModelHandle()const;

private:
	/*�����o�ϐ�*/
	ColliderData* collider;
	int modelHandle;
};

