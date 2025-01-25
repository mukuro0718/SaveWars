#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_SweepBreathSmash.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_SweepBreathSmash::Dragon_SweepBreathSmash()
	: useAnimationType	(0)
	, maxUseAnimation	(0)
	, nearAttackRange	(0.0f)
	, FIX_ROTATE_FRAME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["FIX_ROTATE_FRAME"])
	, frameCount		(0)
	, isClose			(false)
{
	/*�g�p����A�j���[�V����*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SWEEP));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));

	/*�U���̓����蔻��̃^�C�~���O�}�b�v*/
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::SWEEP), static_cast<short>(Dragon::AttackCollider::SWEEP));
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::BREATH), static_cast<short>(Dragon::AttackCollider::BREATH));
	this->useColliderIndex.emplace(static_cast<short>(UseAnimationType::SMASH), static_cast<short>(Dragon::AttackCollider::SMASH));

	/*�g�p����A�j���[�V�����̍Đ�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime	= json.GetJson(JsonManager::FileType::DRAGON)["SWEEP_BREATH_SMASH_PLAY_TIME"];
	this->animationPlayTime = playTime;
	this->maxUseAnimation	= static_cast<int>(UseAnimationType::SMASH);
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::SWEEP_BREATH_SMASH);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange	= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_SweepBreathSmash::~Dragon_SweepBreathSmash()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_SweepBreathSmash::NodeState Dragon_SweepBreathSmash::Update()
{
	/*�ړ�����*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	short walkIndex1 = static_cast<short>(UseAnimationType::WALK_1);
	short walkIndex2 = static_cast<short>(UseAnimationType::WALK_2);
	if (this->useAnimationType == walkIndex1 || this->useAnimationType == walkIndex2 || !this->isFixRotate)
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
	}
	else
	{
		enemy.UpdateSpeed(0.0f, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.GetCurrentAction() != this->actionType)
	{
		//�A�N�V�����̐ݒ�
		rootNode.SetCurrentAction(this->actionType);
		//�A�N�V�����̓o�^
		rootNode.EntryCurrentBattleAction(*this);
	}

	/*�t���[���J�E���g�̑���*/
	if (!this->isFixRotate)
	{
		this->frameCount++;
		if (this->frameCount >= this->FIX_ROTATE_FRAME)
		{
			this->isFixRotate = true;
		}
	}

	/*�����蔻��R���C�_�[�̍X�V*/
	//�w��̃A�j���[�V��������Ȃ����
	if (this->useAnimationType != walkIndex1 || this->useAnimationType != walkIndex2)
	{
		enemy.UpdateAttackCollider(this->useColliderIndex[this->useAnimationType], this->nowTotalPlayTime);
	}

	/*�A�j���[�V�����̍Đ�*/
	float playTime = this->animationPlayTime[this->useAnimationType];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType[this->useAnimationType], playTime);

	/*��Ԃ�Ԃ�*/
	//�ړ���Ԃ��ߐڍU���͈͂Ȃ玟�̃A�j���[�V�����Ɉڍs����
	if (this->useAnimationType == walkIndex1 || this->useAnimationType == walkIndex2)
	{
		if (rootNode.GetToTargetDistance() < this->nearAttackRange)
		{
			this->isClose = true;
			this->useAnimationType++;
			this->nowTotalPlayTime = 0.0f;
		}
	}
	//�A�j���[�V�������I�����Ă�����
	else if (enemy.GetIsChangeAnimation())
	{
		if (this->useAnimationType == static_cast<short>(UseAnimationType::SWEEP))
		{
			if (rootNode.GetToTargetDistance() < this->nearAttackRange)
			{
				this->useAnimationType = static_cast<short>(UseAnimationType::WALK_2);
				return ActionNode::NodeState::RUNNING;
			}
		}
		enemy.OffAttackCollider(this->useColliderIndex[this->useAnimationType]);
		this->nowTotalPlayTime = 0.0f;
		//��]�U���A�j���[�V�������I�����Ă����琬����Ԃ�
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->isClose = false;
			this->isFixRotate = false;
			this->frameCount = 0;
			this->useAnimationType = 0;
			//�A�N�V�����̉���
			rootNode.ExitCurrentBattleAction();
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�Ȃ玟�̃A�j���[�V�����Ɉڍs����
		this->useAnimationType++;
		return ActionNode::NodeState::RUNNING;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
