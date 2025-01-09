//====================================================
// @brief �����i�N�g�p�r�w�C�r�A�c���[
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class DragonBehaviorTree : public Singleton<DragonBehaviorTree>
{
public:
	friend class Singleton<DragonBehaviorTree>;

	/*�񋓑�*/
	//�L�����N�^�[�̏��
	enum class DragonState
	{
		NORMAL = 0,//�ʏ�
		TIRED = 1,//�X�^�~�i���؂�Ă�����
	};
	//�A�N�V�����̎��
	enum class ActionType
	{
		DYING					  = 0, //�f�X
		IDLE					  = 1, //�ҋ@
		ROAR					  = 2, //���K(�ʏ큨�{���Ԃ̕ω�)
		WALK					  = 3, //����
		TURN_LEFT				  = 4, //��������
		TURN_RIGHT				  = 5, //�E������
		BREATH_SMASH			  = 6, //�u���X�{�@����
		SMASH_ROTATE			  = 7, //�@�����{��]�U��
		SWEEP_SMASH				  = 8, //�Ȃ������{�@����
		ROTATE_SWEEP			  = 9,//��]�U���{�Ȃ�����
		SWEEP_BREATH			  = 10,//�Ȃ������{�u���X
		BREATH_SMASH_ROTATE		  = 11,//�u���X�{�@�����{��]�U��
		ROTATE_SWEEP_SMASH		  = 12,//��]�U���{�Ȃ������{�@����
		SWEEP_BREATH_SMASH		  = 13,//�Ȃ������{�u���X�{�@����
		BREATH_SMASH_SWEEP_ROTATE = 14,//�u���X�{�@�����{�Ȃ������{��]�U��
		SWEEP_SMASH_ROTATE_BREATH = 15,//�Ȃ������{�@�����{��]�U���{�u���X
		ROTATE_SWEEP_BREATH_SMASH = 16,//��]�U���{�Ȃ������{�u���X�{�@����
	};
		  void Initialize	();		//������
		  void Update		();		//�X�V
	const void Draw			()const;//�`��

	/*getter/setter*/
	const int	GetInterval			(const int _index)const { return this->intervalSet[_index]; }			 //�C���^�[�o���̎擾
	const float GetToTargetDistance	()const					{ return this->toTargetDistance; }				 //�ڕW�ւ̃x�N�g�����擾
	const float GetInnerProduct		()const					{ return this->innerProductOfDirectionToTarget; }//�ڕW�ւ̃x�N�g���̓��ς��擾
	const int	GetCurrentAction	()const					{ return this->currentAction; }					 //���݂̃A�N�V�������擾
	const bool  GetIsActive			()const					{ return this->isActive; }
	void SetCurrentAction(const int _action)					{ this->currentAction = _action; }		//���݂̃A�N�V�����̐ݒ�
	void SetInterval	 (const int _index, const int _set = 0) { this->intervalSet[_index] = _set; }	//�C���^�[�o���̐ݒ�
	void CalcStamina	 (const int _value)						{ this->stamina += _value; }			//�X�^�~�i�̌v�Z
private:
	/*���������֐�*/
		  DragonBehaviorTree	();//�R���X�g���N�^
		 ~DragonBehaviorTree	();//�f�X�g���N�^
	void UpdateMemberVariables	();//�����o�ϐ��̍X�V

	/*�����o�ϐ�*/
	std::array<int, static_cast<int>(ActionType::ROTATE_SWEEP_BREATH_SMASH) + 1>	intervalSet;					//�C���^�[�o��
	BehaviorTreeNode*																Selector_DyingOrBattleOrIdle;	//�r�w�C�r�A�c���[��root�m�[�h
	BehaviorTreeNode*																debugActionNode;				//�f�o�b�N�p�̃A�N�V�����m�[�h(TODO:�}�X�^�łł͏���)
	DragonState																		state;							//�{�X�̏��
	float																			toTargetDistance;				//�ڕW�܂ł̋���
	float																			innerProductOfDirectionToTarget;//�����x�N�g���ƖڕW�x�N�g���̓���
	int																				currentAction;					//���݂̃A�N�V����
	int																				stamina;						//�X�^�~�i
	bool																			isActive;						//�A�N�e�B�u��Ԃ��i�X�^�~�i���؂ꂽ��ő�l�ɂȂ�܂�false:�m���A�N�e�B�u��ԂɂȂ�B�j
	BehaviorTreeNode::NodeState														prevNodeState;					//�O�̃m�[�h����A���Ă������
};