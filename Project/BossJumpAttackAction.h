//=============================================
// @brief �{�X�W�����v�U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossJumpAttackAction : public BossAction
{
public:
	BossJumpAttackAction();
	virtual ~BossJumpAttackAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	bool isClose;//�߂Â�����
};

