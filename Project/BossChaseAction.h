//=============================================
// @brief �{�X�ǐՃA�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossChaseAction : public BossAction
{
public:
	 BossChaseAction();//�R���X�g���N�^
	~BossChaseAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	
};

