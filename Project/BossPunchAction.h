//=============================================
// @brief �{�X�p���`�U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossPunchAction : public BossAction
{
public:
	 BossPunchAction();//�R���X�g���N�^
	~BossPunchAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z

private:

};

