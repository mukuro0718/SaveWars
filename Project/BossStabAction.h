//=============================================
// @brief �{�X�˂��h���U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossStabAction : public BossAction
{
public:
	BossStabAction();
	virtual ~BossStabAction();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
	bool isClose;//�߂Â�����
};

