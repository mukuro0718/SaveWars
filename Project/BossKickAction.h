//=============================================
// @brief �{�X��эU���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossKickAction : public BossAction
{
public:
	BossKickAction();
	virtual ~BossKickAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
};

