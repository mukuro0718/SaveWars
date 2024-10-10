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
	virtual ~BossRestAction();//�f�X�g���N�^

	void Initialize()				  override;//������
	void Update(Boss& _boss)	override;//�p�����[�^�̌v�Z
	void CalcParameter(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	enum class DirectionType
	{
		LEFT,
		RIGHT,
	};

	bool isSetMoveDirection;
	int directionType;
	int maxFrameCount;
};

