//=============================================
// @brief �{�X�ǐՃA�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossChaseAction : public BossAction
{
public:
	BossChaseAction();
	virtual ~BossChaseAction();

	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	
};

