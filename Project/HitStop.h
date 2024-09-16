//==================================================
// @brief �q�b�g�X�g�b�v�N���X
//==================================================
#pragma once
#include "Singleton.h"

class HitStop : public Singleton<HitStop>
{
public:
	friend class Singleton<HitStop>;

	void Initialize();//������
	const void Draw()const;//�`��
	bool IsHitStop();//�q�b�g�X�g�b�v���s���Ă��邩

	/*setter*/
	void SetHitStopTime(const float _time) { this->time = _time; }//�q�b�g�X�g�b�v���Ԃ�ݒ�
private:
	/*���������֐�*/
	HitStop();//�R���X�g���N�^
	~HitStop();//�f�X�g���N�^

	/*�����o�ϐ�*/
	int time;//�q�b�g�X�g�b�v����
};

