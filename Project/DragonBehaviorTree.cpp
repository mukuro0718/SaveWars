#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
	: Selector_DyingOrBattleOrIdle	 (nullptr)
	, debugActionNode				 (nullptr)
	, currentBattleAction			 (nullptr)
	, state							 (DragonState::NORMAL)
	, prevNodeState					 (BehaviorTreeNode::NodeState::NONE_ACTIVE)
	, toTargetDistance				 (0.0f)
	, innerProductOfDirectionToTarget(0.0f)
	, currentAction					 (0)
	, stamina						 (0)
	, isActive						 (true)
	, isSelectedBattleAction		 (false)
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�匳�̃c���[�̍쐬*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*�f�X�m�[�h*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanZero());
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());

	/*�o�g���A�N�V����*/
	////�u���X�{�@����
	//BehaviorTreeNode* Sequencer_BreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH)));
	//Sequencer_BreathSmashIfIntervalIsOver->AddChild(*new Dragon_BreathSmash());
	////�@�����{��]�U��
	//BehaviorTreeNode* Sequencer_SmashRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_SmashRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SMASH_ROTATE)));
	//Sequencer_SmashRotateIfIntervalIsOver->AddChild(*new Dragon_SmashRotate());
	////�Ȃ������{�@����
	//BehaviorTreeNode* Sequencer_SweepSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_SMASH)));
	//Sequencer_SweepSmashIfIntervalIsOver->AddChild(*new Dragon_SweepSmash());
	////��]�U���{�Ȃ�����
	//BehaviorTreeNode* Sequencer_RotateSweepIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP)));
	//Sequencer_RotateSweepIfIntervalIsOver->AddChild(*new Dragon_RotateSweep());
	////�Ȃ������{�u���X
	//BehaviorTreeNode* Sequencer_SweepBreathIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_BREATH)));
	//Sequencer_SweepBreathIfIntervalIsOver->AddChild(*new Dragon_SweepBreath());
	////�u���X�{�@�����{��]�U��
	//BehaviorTreeNode* Sequencer_BreathSmashRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH_ROTATE)));
	//Sequencer_BreathSmashRotateIfIntervalIsOver->AddChild(*new Dragon_BreathSmashRotate());
	////��]�U���{�Ȃ������{�@����
	//BehaviorTreeNode* Sequencer_RotateSweepSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP_SMASH)));
	//Sequencer_RotateSweepSmashIfIntervalIsOver->AddChild(*new Dragon_RotateSweepSmash());
	////�Ȃ������{�u���X�{�@����
	//BehaviorTreeNode* Sequencer_SweepBreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_BREATH_SMASH)));
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_SweepBreathSmash());
	////�u���X�{�@�����{�Ȃ������{��]�U��
	//BehaviorTreeNode* Sequencer_BreathSmashSweepRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashSweepRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH_SWEEP_ROTATE)));
	//Sequencer_BreathSmashSweepRotateIfIntervalIsOver->AddChild(*new Dragon_BreathSmashSweepRotate());
	////��]�U���{�Ȃ������{�u���X�{�@����
	//BehaviorTreeNode* Sequencer_RotateSweepBreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP_BREATH_SMASH)));
	//Sequencer_RotateSweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_RotateSweepBreathSmash());
	////�Ȃ������{�@�����{��]�U���{�u���X
	//BehaviorTreeNode* Sequencer_SweepSmashRotateBreathIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_SMASH_ROTATE_BREATH)));
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_SweepSmashRotateBreath());
	//1�i�K�ڂ̍U���A�N�V����
	BehaviorTreeNode* Selector_OneStepAttackAction = new RandomSelector();
	Selector_OneStepAttackAction->AddChild(*new Dragon_BreathSmash());	//�u���X�{�@����
	Selector_OneStepAttackAction->AddChild(*new Dragon_SmashRotate());	//�@�����{��]�U��
	Selector_OneStepAttackAction->AddChild(*new Dragon_SweepSmash());	//�Ȃ������{�@����
	Selector_OneStepAttackAction->AddChild(*new Dragon_RotateSweep());	//��]�U���{�Ȃ�����
	Selector_OneStepAttackAction->AddChild(*new Dragon_SweepBreath());	//�Ȃ������{�u���X
	//2�i�K�ڂ̍U���A�N�V����
	BehaviorTreeNode* Selector_TwoStepAttackAction = new RandomSelector();
	Selector_TwoStepAttackAction->AddChild(*new Dragon_BreathSmashRotate());	//�u���X�{�@�����{��]�U��
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SmashRotate());	//�@�����{��]�U��
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SweepSmash());	//�Ȃ������{�@����
	Selector_TwoStepAttackAction->AddChild(*new Dragon_RotateSweepSmash());	//��]�U���{�Ȃ������{�@����
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SweepBreathSmash());	//�Ȃ������{�u���X�{�@����
	//3�i�K�ڂ̍U���A�N�V����
	BehaviorTreeNode* Selector_ThreeStepAttackAction = new RandomSelector();
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_BreathSmashSweepRotate());//�u���X�{�@�����{�Ȃ������{��]�U��
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SmashRotate());//�@�����{��]�U��
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SweepSmashRotateBreath());//�Ȃ������{�@�����{��]�U���{�u���X
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_RotateSweepBreathSmash());//��]�U���{�Ȃ������{�u���X�{�@����
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SweepBreathSmash());//�Ȃ������{�u���X�{�@����
	//�i�K���Ƃ̍U���A�N�V����
	BehaviorTreeNode* Selector_StepByStepAttackAction = new SelectorNode();
	Selector_StepByStepAttackAction->AddChild(*Selector_OneStepAttackAction);
	Selector_StepByStepAttackAction->AddChild(*Selector_TwoStepAttackAction);
	Selector_StepByStepAttackAction->AddChild(*Selector_ThreeStepAttackAction);
	//�A�N�e�B�u�t���O�������Ă���Ƃ��̃A�N�V����
	BehaviorTreeNode* Sequencer_ActiveActionIfActiveFlagIsTrue = new SequencerNode();
	Sequencer_ActiveActionIfActiveFlagIsTrue->AddChild(*new Condition_IsDragonActiveFlagIsTrue());
	Sequencer_ActiveActionIfActiveFlagIsTrue->AddChild(*Selector_StepByStepAttackAction);
	//�A�N�V�������I������Ă��邩
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedDragonBattleAction());
	Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	//�A�N�e�B�u���ǂ����𔻒f���A�N�V���������߂�Z���N�^�[�m�[�h
	BehaviorTreeNode* Selector_ActiveOrNoneActiveAction = new SelectorNode();
	Selector_ActiveOrNoneActiveAction->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	Selector_ActiveOrNoneActiveAction->AddChild(*Sequencer_ActiveActionIfActiveFlagIsTrue);
	Selector_ActiveOrNoneActiveAction->AddChild(*Selector_OneStepAttackAction);

	/*���[�g�m�[�h�Ɋe�A�N�V�����c���[��}��*/
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Selector_ActiveOrNoneActiveAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*new Dragon_Idle());

	this->debugActionNode = new Dragon_RotateSweepBreathSmash();

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{

}

/// <summary>
/// ������
/// </summary>
void DragonBehaviorTree::Initialize()
{
	//auto& json	= Singleton<JsonManager>::GetInstance();
	this->state		= DragonState::NORMAL;
}

/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*�C���^�[�o���̌v�Z*/
	for (auto interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*�ڕW�܂ł̋��������߂�*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*�{�X�̌����Ă�������ƃv���C���[�̈ʒu�Ƃ̓��ς����*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// �X�V
/// </summary>
void DragonBehaviorTree::Update()
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables();

	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->currentAction);

	/*�c���[�̎��s*/
	this->prevNodeState = this->Selector_DyingOrBattleOrIdle->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// �`��
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	this->debugActionNode->Draw();
#endif // _DEBUG

}

/// <summary>
/// �o�g���A�N�V�����̓o�^
/// </summary>
void DragonBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true;
}

/// <summary>
/// �o�g���A�N�V�����̉���
/// </summary>
void DragonBehaviorTree::ExitCurrentBattleAction()
{
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false;
}