//=============================================
// @brief �{�X��]�R��U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlapAction : public BossAction
{
public:
	BossSlapAction();
	virtual ~BossSlapAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:

};

