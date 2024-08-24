//========================================
// @brief �}�b�v�}�l�[�W���[�N���X
// �}�b�v�̊Ǘ����s��
//========================================
#pragma once
#include "Singleton.h"

class HP;
class UIManager : public Singleton<UIManager>
{
public:
	friend class Singleton<UIManager>;

	void Initialize();		//������
	void Update();		//�X�V
	const void Draw()const;//�`��

private:
	/*���������֐�*/
	UIManager();//�R���X�g���N�^
	~UIManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	HP* hp;
};

