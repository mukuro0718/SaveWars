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

	void		Initialize();		//������
	void		Finalize();		//�㏈��
	void		Update();		//�X�V
	const void	Draw()const;	//�`��

private:
	/*�����o�ϐ�*/
	int modelHandle;
	ColliderData* collider;
};

