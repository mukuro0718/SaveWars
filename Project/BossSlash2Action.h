//=============================================
// @brief �{�X�X���b�V���U���A�N�V����Ver.2
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlash2Action : public BossAction
{
public:
	BossSlash2Action();
	virtual ~BossSlash2Action();

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:

};

