//=============================================
// @brief �{�X��]�R��U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashCombo2Action : public BossAction
{
public:
	BossSlashCombo2Action();
	virtual ~BossSlashCombo2Action();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
	bool isClose;//��x�ł��߂Â�����
};

