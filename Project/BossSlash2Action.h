//=============================================
// @brief �{�X�X���b�V���U���A�N�V����Ver.2
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlash2Action : public BossAction
{
public:
	 BossSlash2Action();//�R���X�g���N�^
	~BossSlash2Action();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	bool isClose;//�߂Â�����
};

