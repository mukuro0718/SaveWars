//=============================================
// @brief �{�X�X���b�V���U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashAction : public BossAction
{
public:
	BossSlashAction();
	virtual ~BossSlashAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
};

