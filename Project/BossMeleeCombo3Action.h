//=============================================
// @brief�@�����[�R���{�R�A�N�V�����p�����[�^�[
//=============================================
#pragma once

class Boss;
class BossAction;
class BossMeleeCombo3Action : public BossAction
{
public:
	BossMeleeCombo3Action();
	virtual ~BossMeleeCombo3Action();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:

};

