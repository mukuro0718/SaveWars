//=============================================
// @brief �{�X��]�p���`�U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossRotatePunchAction : public BossAction
{
public:
	BossRotatePunchAction();
	virtual ~BossRotatePunchAction();

	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	bool isClose;//�߂Â�����
};

