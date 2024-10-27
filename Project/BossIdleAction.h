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
	virtual ~BossIdleAction();//�f�X�g���N�^

	void Initialize()						override;//������
	void Update(Boss& _boss)				override;//�p�����[�^�̌v�Z
	void CalcParameter(const Boss& _boss)	override;//�p�����[�^�[�̌v�Z
private:
	/*�񋓑�*/
	enum class DirectionType//����
	{
		LEFT = 0,
		RIGHT = 1,
	};

	/*�����o�ϐ�*/
	bool isSetMoveDirection;//�������Z�b�g������
	int  directionType;		//�����̎��
	int  maxFrameCount;		//�t���[���J�E���g
};

