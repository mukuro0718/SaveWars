//============================================
// @brief �{�X���K�p�����[�^
//============================================
#pragma once

class Boss;
class BossAction;
class BossRoarAction : public BossAction
{
public:
	 BossRoarAction();//�R���X�g���N�^
	~BossRoarAction();//�f�X�g���N�^
	
	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�p�����[�^�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	static constexpr char FLAG_YES = 1 << 0;
	int	 prevState;		//�O�̓{����
	char isFinishedFirstRoar;	//�ŏ��̙��K���I���������t���O
	char isInitializeColorScale;//�J���[�X�P�[����������������
};

