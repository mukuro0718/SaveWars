//============================================
// @brief �{�X�f�X�p�����[�^
//============================================
#pragma once

class Boss;
class BossAction;
class BossDeathAction : public BossAction
{
public:
	 BossDeathAction();//�R���X�g���N�^
	~BossDeathAction();//�f�X�g���N�^

	void Initialize	  ()					override;//������
	void Update		  (Boss& _boss)			override;//�X�V
	void CalcParameter(const Boss& _boss)	override;//�p�����[�^�[�̌v�Z
};

