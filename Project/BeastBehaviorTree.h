//====================================================
// @brief ���N�X���I�p�r�w�C�r�A�c���[
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class BeastBehaviorTree : public Singleton<BeastBehaviorTree>
{
public:
	friend class Singleton<BeastBehaviorTree>;

	/*�񋓑�*/
	//�L�����N�^�[�̏��
	enum class BeastState
	{
		ANGRY		= 0,
		NORMAL		= 1,
		DOWN		= 2,
		FRIGHTENING = 3,
		BREAK		= 4,
	};

	enum class ActionType
	{
		DYING				= 0, //�f�X
		IDLE				= 1, //�ҋ@
		ROAR				= 2, //���K(�ʏ큨�{���Ԃ̕ω�)
		WALK				= 3, //����
		RUN					= 4, //�_�b�V��
		TURN_LEFT			= 5, //��������
		TURN_RIGHT			= 6, //�E������
		DOWN				= 7, //�_�E��
		LONG_FRIGHT			= 8, //��������
		SHORT_FRIGHT		= 9, //�Z������
		REST				= 10,//�x�e
		STEP_BACK			= 11,//���ɃW�����v���ĉ�����
		RAISE_LEVEL			= 12,//���̃`���[�W�����㏸������i���K�j
		BACKING_BREATH		= 13,//���ɉ�����Ȃ���u���X
		RUSH				= 14,//�ːi
		CHARGE_RIGHT_ATTACK	= 15,//�E�O�����ߍU��
		CHARGE_BOTH_ATTACK	= 16,//���O�����ߍU��
		BREATH				= 17,//�u���X
		SUPER_NOVA			= 18,//�X�[�p�[�m���@
		WEAK_BREATH			= 19,//��u���X
		SMALL_EXPLOSION		= 20,//������
		RIGHT_FOOT_ATTACK	= 21,//�E���U��
		COMBO_ATTACK_1		= 22,//�E���ōU���������]�U��
		COMBO_ATTACK_2		= 23,//3�A���U��
	};
	//���x���̒i�K
	enum class LevelStage
	{
		Lv1 = 0,
		Lv2 = 1,
		Lv3 = 2,
		Lv4 = 3,
	};
	void Initialize	();//������
	void Update		();//�X�V

	/*getter/setter*/
	const int		 GetDownValue		()const				{ return this->downValue; }				//�_�E���l�̎擾
	const int		 GetDamage			()const				{ return this->damage; }				//�_���[�W�̎擾
	const int		 GetPrevHp			()const				{ return this->prevHp; }				//�O�t���[����HP�̎擾
	const BeastState GetBeastState		()const				{ return this->state; }					//�{�X�̏�Ԃ��擾
	const int		 GetInterval		(const int _index)	{ return this->intervalSet[_index]; }	//�C���^�[�o���̎擾
	const float		 GetToTargetDistance()const				{ return this->toTargetDistance; }		//�ڕW�܂ł̋������擾
	const int		 GetLevel			()const				{ return this->level; }					//���x���̎擾
	void SetDownValue		(const int _set)						{ this->downValue = _set; }				//�_�E���l���Z�b�g
	void SetDamage			(const int _set)						{ this->damage = _set; }				//�_���[�W���Z�b�g
	void SetPrevHp			(const int _set)			 			{ this->downValue = _set; }				//�O�t���[����HP���Z�b�g
	void SetBeastState		(const BeastState _state)				{ this->state = _state; }				//�{�X�̏�Ԃ�ݒ�
	void SetInterval		(const int _index,const int _set = 600)	{ this->intervalSet[_index] = _set; }	//�C���^�[�o���̃Z�b�g�i�j
	void SetToTargetDistance(const float _set)						{ this->toTargetDistance = _set; }		//�ڕW�܂ł̋������Z�b�g
	void RaiseLevel() { this->level++; }	//���x���㏸
	void ResetLevel() { this->level = 0; }	//���x���̃��Z�b�g
private:
	/*���������֐�*/
	 BeastBehaviorTree();//�R���X�g���N�^
	~BeastBehaviorTree();//�f�X�g���N�^

	/*�����o�ϐ�*/
	BehaviorTreeNode* root;		//�c���[
	BeastState		  state;	//�{�X�̏��
	int				  downValue;//�_�E���l
	std::array<int, static_cast<int>(ActionType::COMBO_ATTACK_2) + 1> intervalSet;//�C���^�[�o��
	int prevHp;//�O�t���[����HP
	int damage;//���܂łɎ󂯂��_���[�W�ʁi�{���Ԃ��I������ƂO�Ƀ��Z�b�g����j
	int level;
	float toTargetDistance;//�ڕW�܂ł̋���
};

/*
root--DyingNode
	|-ActionNode--Transition
				|-Recovering
				|-Serch
				|-Battle
*/