#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BeastBehaviorTree::BeastBehaviorTree()
	: Selector_DeathOrReactionOrBattleOrBreak	(nullptr)
	, state										(BeastState::NORMAL)
	, prevNodeState								(BehaviorTreeNode::NodeState::NONE_ACTIVE)
	, downValue									(0)
	, prevHp									(0)
	, damage									(0)
	, level										(0)
	, toTargetDistance							(0.0f)
	, isDestroyedPart							(false)
	, selectAction								(-1)
	, isSelectedBattleAction					(false)
	, isSelectedReaction						(false)
	, currentBattleAction						(nullptr)
	, currentReaction							(nullptr)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	/*�C���^�[�o����������*/
	this->intervalSet = { 0 };

	/*�匳�̃c���[�̍쐬*/
	this->Selector_DeathOrReactionOrBattleOrBreak = new SelectorNode();

	/*HP���Ȃ��Ƃ��̃A�N�V����*/
	{
		//�f�X����m�[�h
		BehaviorTreeNode* Sequencer_DeathIfHpIsLessThanZero = new SequencerNode();
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Condition_IsHpBelowConstant(0));
		Sequencer_DeathIfHpIsLessThanZero->AddChild(*new Beast_Dying());
		//root�m�[�h�ɒǉ�
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Sequencer_DeathIfHpIsLessThanZero);
	}

	/*���A�N�V�������̃A�N�V����*/
	{
		//�T�u�c���[
		BehaviorTreeNode* Selector_DownOrLongOrShortReaction = new SelectorNode();
		//�T�u�c���[�ɂԂ牺����q�m�[�h
		{
			//���A�N�V�������I������Ă�����
			BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedReaction = new SequencerNode();
			Sequencer_PlayCurrentIfSelectedReaction->AddChild(*new Condition_IsSelectedReaction());
			Sequencer_PlayCurrentIfSelectedReaction->AddChild(*new Beast_PlayCurrentReaction());
			//�_�E��
			BehaviorTreeNode* Sequencer_DownIfDownValueMoreThanConstant = new SequencerNode();
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::DOWN));
			Sequencer_DownIfDownValueMoreThanConstant->AddChild(*new Beast_Down());
			//��������
			BehaviorTreeNode* Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant = new SequencerNode();
			Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant->AddChild(*new Condition_IsFrighteningValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["SPECIFIED_FRIGHTENING_VALUE"]));
			Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant->AddChild(*new Beast_LongFrightening());
			//�Z������
			BehaviorTreeNode* Sequencer_ShortFrighteningIfPartDestroyed = new SequencerNode();
			Sequencer_ShortFrighteningIfPartDestroyed->AddChild(*new Condition_IsDestoroyedPart());
			Sequencer_ShortFrighteningIfPartDestroyed->AddChild(*new Beast_ShortFrightening());
			//�T�u�c���[�ǉ�
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_PlayCurrentIfSelectedReaction);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_DownIfDownValueMoreThanConstant);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_LongFrighteningIfFrighteningValueMoreThanConstant);
			Selector_DownOrLongOrShortReaction->AddChild(*Sequencer_ShortFrighteningIfPartDestroyed);
		}
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_DownOrLongOrShortReaction);
	}

	/*�퓬���̃A�N�V����*/
	{
		//�T�u�c���[
		BehaviorTreeNode* Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack = new SelectorNode();

		//�ړ��T�u�c���[
		BehaviorTreeNode* Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack = new SequencerNode();
		{
			//�ˌ�
			BehaviorTreeNode* Sequencer_RushIfIntervalOver = new SequencerNode();
			Sequencer_RushIfIntervalOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RUSH)));
			Sequencer_RushIfIntervalOver->AddChild(*new Beast_Rush());
			//����
			BehaviorTreeNode* Sequencer_WalkIfStateNormal = new SequencerNode();
			Sequencer_RushIfIntervalOver->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
			Sequencer_RushIfIntervalOver->AddChild(*new Beast_Walk());
			//�A�N�V�����I���m�[�h
			BehaviorTreeNode* Selector_TurnOrRushOrWalkOrRun = new SelectorNode();
			Sequencer_RushIfIntervalOver->AddChild(*Sequencer_RushIfIntervalOver);
			Sequencer_RushIfIntervalOver->AddChild(*Sequencer_WalkIfStateNormal);
			Sequencer_RushIfIntervalOver->AddChild(*new Beast_Run());
			//�ړ��T�u�c���[�ɒǉ�
			Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["FAR_ATTACK_RANGE"]));
			Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack->AddChild(*Selector_TurnOrRushOrWalkOrRun);
		}
		//��]
		BehaviorTreeNode* Sequencer_TurnIfTargetOutOfRangeRay = new SequencerNode();
		Sequencer_TurnIfTargetOutOfRangeRay->AddChild(*new Condition_IsTargetOutOfRangeOfRay(json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][0], json.GetJson(JsonManager::FileType::BEAST)["TOLERANCE_RANGE"][1]));
		Sequencer_TurnIfTargetOutOfRangeRay->AddChild(*new Beast_TurnLeft());
		//����U���T�u�c���[
		BehaviorTreeNode* Sequencer_SpecialActionIfIntervalIsOver = new SequencerNode();
		{
			//�X�[�p�[�m���@
			BehaviorTreeNode* Sequencer_SuperNovaIfLevelIsMax = new SequencerNode();
			Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv4)));
			Sequencer_SuperNovaIfLevelIsMax->AddChild(*new Beast_SuperNova());
			//�A�N�V�����I���m�[�h
			BehaviorTreeNode* Selector_SuperNovaOrRaiseLevel = new SelectorNode();
			Selector_SuperNovaOrRaiseLevel->AddChild(*Sequencer_SuperNovaIfLevelIsMax);
			Selector_SuperNovaOrRaiseLevel->AddChild(*new Beast_RaiseLevel());
			//����U���T�u�c���[�ɒǉ�
			Sequencer_SpecialActionIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RAISE_LEVEL)));
			Sequencer_SpecialActionIfIntervalIsOver->AddChild(*Selector_SuperNovaOrRaiseLevel);
		}
		//�������U���T�u�c���[
		BehaviorTreeNode* Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
		{
			//�u���X
			BehaviorTreeNode* Sequencer_BreathIfLevelFourAndIntervalIsOver = new SequencerNode();
			Sequencer_BreathIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv3)));
			Sequencer_BreathIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::BREATH)));
			Sequencer_BreathIfLevelFourAndIntervalIsOver->AddChild(*new Beast_Breath());
			//��ނ��Ȃ���u���X
			BehaviorTreeNode* Sequencer_BackingIfLevelFourAndIntervalIsOver = new SequencerNode();
			Sequencer_BackingIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv2)));
			Sequencer_BackingIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::BACKING_BREATH)));
			Sequencer_BackingIfLevelFourAndIntervalIsOver->AddChild(*new Beast_BackingBreath());
			//��u���X
			BehaviorTreeNode* Sequencer_WeakIfLevelFourAndIntervalIsOver = new SequencerNode();
			Sequencer_WeakIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsNowLevelIsSameAsTheSpecifiedLevel(static_cast<int>(LevelStage::Lv1)));
			Sequencer_WeakIfLevelFourAndIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::WEAK_BREATH)));
			Sequencer_WeakIfLevelFourAndIntervalIsOver->AddChild(*new Beast_WeakBreath());
			//�A�N�V�����I���m�[�h
			BehaviorTreeNode* Selector_BreathOrBackingOrExplosionOrWeak = new SelectorNode();
			Selector_BreathOrBackingOrExplosionOrWeak->AddChild(*Sequencer_BreathIfLevelFourAndIntervalIsOver);
			Selector_BreathOrBackingOrExplosionOrWeak->AddChild(*Sequencer_BackingIfLevelFourAndIntervalIsOver);
			Selector_BreathOrBackingOrExplosionOrWeak->AddChild(*Sequencer_WeakIfLevelFourAndIntervalIsOver);
			//�������U���T�u�c���[�ɒǉ�
			Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["NEAR_ATTACK_RANGE"]));
			Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack->AddChild(*Selector_BreathOrBackingOrExplosionOrWeak);
		}
		//�ߋ����U���T�u�c���[
		BehaviorTreeNode* Selector_WalkOrNormalActionOrAngryAction = new SelectorNode();
		{
			//�X�e�b�v�o�b�N
			BehaviorTreeNode* Sequencer_StepBackIfIntervalIsOver = new SequencerNode();
			Sequencer_StepBackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::STEP_BACK)));
			Sequencer_StepBackIfIntervalIsOver->AddChild(*new Beast_StepBack());
			//�E���U��
			BehaviorTreeNode* Sequencer_RightAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_RightAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::RIGHT_FOOT_ATTACK)));
			Sequencer_RightAttackIfIntervalIsOver->AddChild(*new Beast_RightFootAttack());
			//�R���{�A�^�b�N
			BehaviorTreeNode* Sequencer_ComboAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_ComboAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::COMBO_ATTACK)));
			Sequencer_ComboAttackIfIntervalIsOver->AddChild(*new Beast_FootComboAttack());
			//�E���ߍU��
			BehaviorTreeNode* Sequencer_ChargeRightAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_ChargeRightAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::CHARGE_RIGHT_ATTACK)));
			Sequencer_ChargeRightAttackIfIntervalIsOver->AddChild(*new Beast_ChargeRightFootAttack());
			//�����U��
			BehaviorTreeNode* Sequencer_BothAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_BothAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::CHARGE_BOTH_ATTACK)));
			Sequencer_BothAttackIfIntervalIsOver->AddChild(*new Beast_ChargeBothFootAttack());
			//�E�����]�U��
			BehaviorTreeNode* Sequencer_RightAndRotAttackIfIntervalIsOver = new SequencerNode();
			Sequencer_RightAndRotAttackIfIntervalIsOver->AddChild(*new Condition_IsActionIntervalIsOver(static_cast<int>(ActionType::EXPLOSION)));
			Sequencer_RightAndRotAttackIfIntervalIsOver->AddChild(*new Beast_Explosion());

			//�ʏ펞�̍U���m�[�h
			BehaviorTreeNode* Selector_RightOrComboAttack = new SelectorNode();
			Selector_RightOrComboAttack->AddChild(*Sequencer_RightAttackIfIntervalIsOver);
			Selector_RightOrComboAttack->AddChild(*Sequencer_ComboAttackIfIntervalIsOver);
			//�{�莞�̍U���m�[�h
			BehaviorTreeNode* Selector_ChargeOrBothOrRightAndRotAttack = new SelectorNode();
			//Selector_ChargeOrBothOrRightAndRotAttack->AddChild(*Sequencer_StepBackIfIntervalIsOver);
			Selector_ChargeOrBothOrRightAndRotAttack->AddChild(*Sequencer_BothAttackIfIntervalIsOver);
			Selector_ChargeOrBothOrRightAndRotAttack->AddChild(*Sequencer_RightAndRotAttackIfIntervalIsOver);

			//�ʏ펞�̃A�N�V�����I���m�[�h
			BehaviorTreeNode* Sequencer_NormalActionIfStateNormal = new SequencerNode();
			Sequencer_NormalActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::NORMAL));
			Sequencer_NormalActionIfStateNormal->AddChild(*Selector_RightOrComboAttack);
			//�{�莞�̃A�N�V�����I���m�[�h
			BehaviorTreeNode* Sequencer_AngryActionIfStateNormal = new SequencerNode();
			Sequencer_AngryActionIfStateNormal->AddChild(*new Condition_IsNowStateIsSameAsTheSpecifiedState(BeastState::ANGRY));
			Sequencer_AngryActionIfStateNormal->AddChild(*Selector_ChargeOrBothOrRightAndRotAttack);
			//����
			BehaviorTreeNode* Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack = new SequencerNode();
			Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Condition_IsToTargetDistanceGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["NEAR_ATTACK_RANGE"]));
			Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack->AddChild(*new Beast_Walk());
			//�A�N�V�����I���m�[�h
			Selector_WalkOrNormalActionOrAngryAction->AddChild(*Sequencer_WalkIfTargetOutOfRangeOfNearRangeAttack);
			Selector_WalkOrNormalActionOrAngryAction->AddChild(*Sequencer_NormalActionIfStateNormal);
			Selector_WalkOrNormalActionOrAngryAction->AddChild(*Sequencer_AngryActionIfStateNormal);
		}
		//�A�N�V�������I������Ă��邩
		BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedBattleAction());
		Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Beast_PlayCurrentBattleAction());
		//���K�A�N�V����
		BehaviorTreeNode* Sequencer_RoarIfAngryValueGreaterThanConstant = new SequencerNode();
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Condition_IsAngryValueGreaterThanConstant(json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"]));
		Sequencer_RoarIfAngryValueGreaterThanConstant->AddChild(*new Beast_Roar());
		//�T�u�c���[�ɒǉ�
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_TurnIfTargetOutOfRangeRay);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_ApproachIfTargetOutOfRangeOfLongRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_SpecialActionIfIntervalIsOver);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Sequencer_LongRangeAttackIfTargetOutOfRangeOfNearRangeAttack);
		Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack->AddChild(*Selector_WalkOrNormalActionOrAngryAction);

		//root�m�[�h�ɒǉ�
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Sequencer_RoarIfAngryValueGreaterThanConstant);
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*Selector_ApproachOrSpecialOrLongRangeOrNearRangeAttack);
		this->Selector_DeathOrReactionOrBattleOrBreak->AddChild(*new Beast_Idle());
	}

	this->debugActionNode = new Beast_BackingBreath();
	
	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{

}

/// <summary>
/// ������
/// </summary>
void BeastBehaviorTree::Initialize()
{
	auto& json				= Singleton<JsonManager>::GetInstance();
	this->prevHp			= json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->state				= BeastState::NORMAL;
	this->downValue			= 0;
	this->damage			= 0;
	this->level				= 0;
	this->toTargetDistance	= 0;
	this->selectAction		= -1;
	this->isDestroyedPart	= false;
	this->angryValue = 0;
	for (int i = 0; i < this->intervalSet.size(); i++)
	{
		this->intervalSet[i] = 0;
	}
}

/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void BeastBehaviorTree::UpdateMemberVariables()
{
	/*�C���^�[�o���̌v�Z*/
	for (int& interval : this->intervalSet)
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

	/*HP�̍X�V*/
	this->prevHp = enemy.GetHP();

	/*�{�X�̌����Ă�������ƃv���C���[�̈ʒu�Ƃ̓��ς����*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// �X�V
/// </summary>
void BeastBehaviorTree::Update()
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables();
	ChangeState();

#ifdef _DEBUG
	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->selectAction);
#endif // _DEBUG

	/*�c���[�̎��s*/
	this->prevNodeState = this->Selector_DeathOrReactionOrBattleOrBreak->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// �`��
/// </summary>
const void BeastBehaviorTree::Draw()const
{
#ifdef _DEBUG
	if (this->currentBattleAction)
	{
		this->currentBattleAction->Draw();
	}
	this->debugActionNode->Draw();
#endif // _DEBUG

}

/// <summary>
/// �o�g���A�N�V�����̓o�^
/// </summary>
void BeastBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true; 
}
/// <summary>
/// ���A�N�V�����A�N�V�����̓o�^
/// </summary>
void BeastBehaviorTree::EntryCurrentReaction(BehaviorTreeNode& _action)
{
	this->currentReaction = &_action;
	this->isSelectedReaction = true;
}
/// <summary>
/// ���A�N�V�����A�N�V�����̉���
/// </summary>
void BeastBehaviorTree::ExitCurrentReaction()
{ 
	this->currentReaction->Initialize();
	this->currentReaction = nullptr; 
	this->isSelectedReaction = false; 
}
/// <summary>
/// �o�g���A�N�V�����̉���
/// </summary>
void BeastBehaviorTree::ExitCurrentBattleAction() 
{ 
	this->currentBattleAction->Initialize();
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false; 
}

/// <summary>
/// ��Ԃ̕ύX
/// </summary>
void BeastBehaviorTree::ChangeState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*�{����*/
	switch (this->state)
	{
		//�{��
	case BeastState::ANGRY:
		this->angryValue--;
		if (this->angryValue < 0)
		{
			this->state = BeastState::DOWN;
			this->angryValue = 0;
			this->downValue = json.GetJson(JsonManager::FileType::BEAST)["MAX_DOWN_VALUE"];
		}
		break;
		//�ʏ�
	case BeastState::NORMAL:
		//�{��l�𑝉�
		this->angryValue++;
		//�U�����������Ă�����{��l������ɑ���
		
		if (enemy.GetCharacterData().isHit)
		{
			this->angryValue++;
		}
		//�{��l���ő�ȏゾ�������Ԃ�ANGRY�ɂ���
		if (this->angryValue >= json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"])
		{
			this->angryValue = json.GetJson(JsonManager::FileType::BEAST)["MAX_ANGRY_VALUE"];
			//MV1SetTextureGraphHandle(this->modelHandle, 0, this->angryTexture, FALSE);
		}
		break;
		//���
	case BeastState::DOWN:
		//��ꎞ�Ԃ𑝉�
		this->downValue--;
		////�_�E���l���O�ɂȂ������Ԃ�ʏ�ɂ���
		//if (this->downValue <= 0 )
		//{
		//	this->state = BeastState::NORMAL;
		//	//MV1SetTextureGraphHandle(this->modelHandle, 0, this->normalTexture, FALSE);
		//}
		break;
	}
}