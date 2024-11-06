//==========================================================
// @brief �{�X�x�e�A�N�V�����N���X
// �������Ȃ�
//==========================================================
#pragma once

class Boss;
class BossAction;
class BossRestAction : public BossAction
{
public:
	 BossRestAction();//�R���X�g���N�^
	~BossRestAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	int nowAnimationType;
	int  maxFrameCount;			//�ő�t���[��
	bool isChangeColorScale;	//�J���[�X�P�[����������������
	int	 prevAngryState;		//�O�̓{����
	bool isInitializeColorScale;//�J���[�X�P�[����������������
};

