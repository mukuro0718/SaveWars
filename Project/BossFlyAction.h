//=============================================
// @brief �{�X��эU���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossFlyAction : public BossAction
{
public:
	BossFlyAction();
	virtual ~BossFlyAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:

};

