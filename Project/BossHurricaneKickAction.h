//=============================================
// @brief �{�X��]�R��U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossHurricaneKickAction : public BossAction
{
public:
	BossHurricaneKickAction();
	virtual ~BossHurricaneKickAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:

};

