//=============================================
// @brief�@�X���b�V���R���{�A�N�V����Ver.1
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashComboAction : public BossAction
{
public:
	 BossSlashComboAction();//�R���X�g���N�^
	~BossSlashComboAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�p�����[�^�[�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
};

