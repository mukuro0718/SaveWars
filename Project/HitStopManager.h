//==================================================
// @brief �q�b�g�X�g�b�v�N���X
//==================================================
#pragma once
#include "Singleton.h"

class HitStop;
class HitStopManager : public Singleton<HitStopManager>
{
public:
	friend class Singleton<HitStopManager>;

	void Entry(HitStop& _item);
	void Exit(HitStop& _item);
	void Update();//�X�V
private:
	/*���������֐�*/
	HitStopManager();//�R���X�g���N�^
	~HitStopManager();//�f�X�g���N�^

	/*�ÓI�萔*/
	static constexpr float SLOW_MOTION_FACTOR = 0.3f;

	/*�����o�ϐ�*/
	int type;
	int time;//�q�b�g�X�g�b�v����
	float slowFactor;
	std::list<HitStop*> items;
};

