//=============================================
// @brief �{�X��]�R��U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashCombo2Action : public BossAction
{
public:
	 BossSlashCombo2Action();//�R���X�g���N�^
	~BossSlashCombo2Action();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:
	bool isClose;//��x�ł��߂Â�����
};

