#pragma once
class AttackData abstract
{
public:
	AttackData();
	virtual ~AttackData(){}

	VECTOR hitPosition;
	bool isDoHitCheck;//�����蔻������邩
	bool isHitAttack;//�U��������������
	int damage;//�_���[�W
	//�����ł̃q�b�g�X�g�b�v�n�̕ϐ��́A�L�����N�^�[���ɗ^������̂ɂȂ�
	//�U�����̃q�b�g�X�g�b�v�́A�U���N���X�Őݒ肷��B
	int hitStopTime;
	int hitStopType;
	int hitStopDelay;
	float slowFactor;
};

