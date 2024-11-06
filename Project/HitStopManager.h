//==================================================
// @brief �q�b�g�X�g�b�v�}�l�[�W���[�N���X
//==================================================
#pragma once
#include "Singleton.h"

class HitStop;
class HitStopManager : public Singleton<HitStopManager>
{
public:
	/*HitStopManager��F�B�ɂ���*/
	friend class Singleton<HitStopManager>;

	virtual ~HitStopManager();//�f�X�g���N�^

	void Entry	(HitStop& _item);//�o�^
	void Exit	(HitStop& _item);//����
	void Update	();				 //�X�V
private:
	/*���������֐�*/
	 HitStopManager();//�R���X�g���N�^

	 /*�����o�ϐ�*/
	std::list<HitStop*> items;//�q�b�g�X�g�b�v�N���X�̃��X�g
};

