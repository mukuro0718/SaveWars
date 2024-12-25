//============================================
// @brief �{�X�ҋ@�p�����[�^
//============================================
#pragma once

class Boss;
class BossAction;
class BossIdleAction : public BossAction
{
public:
	 BossIdleAction();//�R���X�g���N�^
	~BossIdleAction();//�f�X�g���N�^

	void Initialize	  ()				  override;//������
	void Update		  (Boss& _boss)		  override;//�X�V
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	/*�ÓI�萔*/
	static constexpr int LEFT = 0;
	static constexpr int RIGHT = 1;

	/*�����o�ϐ�*/
	bool isSetMoveDirection;	//�������Z�b�g������
	int  directionType;			//�����̎��
	int  maxFrameCount;			//�t���[���J�E���g
	bool isChangeColorScale;	//�J���[�X�P�[����ύX���邩
	int	 prevState;				//�O�̏��
	bool isInitializeColorScale;//�J���[�X�P�[����������������
	float speed;
	VECTOR rotateLerpValue;
	short nextAnimation2;
	int maxInterval;
	short checkedState;
};

