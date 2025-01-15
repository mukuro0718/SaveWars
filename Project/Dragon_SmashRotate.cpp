#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_SmashRotate.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_SmashRotate::Dragon_SmashRotate()
	: useAnimationType	 (0)
	, maxUseAnimation	 (0)
	, nearAttackRange	 (0)
	, totalPlayTime		 (0)
	, smashCancelPlayTime(0)
{
	/*�g�p����A�j���[�V����*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK));

	/*�����o�ϐ��̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime		= json.GetJson(JsonManager::FileType::DRAGON)["SMASH_ROTATE_PLAY_TIME"];
	this->animationPlayTime		= playTime;
	this->maxUseAnimation		= static_cast<int>(UseAnimationType::ROTATE);
	this->actionType			= static_cast<short>(DragonBehaviorTree::ActionType::SMASH_ROTATE);
	this->maxSpeed				= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel					= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel					= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange		= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
	this->smashCancelPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["SMASH_CANCEL_PLAY_TIME"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_SmashRotate::~Dragon_SmashRotate()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_SmashRotate::NodeState Dragon_SmashRotate::Update()
{
	/*�ړ�����*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());

	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
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

	/*�A�j���[�V�����̍Đ�*/
	float playTime = this->animationPlayTime[this->useAnimationType];
	enemy.PlayAnimation(this->animationType[this->useAnimationType], playTime);

	/*��Ԃ�Ԃ�*/
	//�ړ���Ԃ��ߐڍU���͈͂Ȃ玟�̃A�j���[�V�����Ɉڍs����
	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
	{
		if (rootNode.GetToTargetDistance() <= this->nearAttackRange)
		{
			this->useAnimationType++;
		}
		return ActionNode::NodeState::RUNNING;
	}
	//�@�����U�����Ɏw��̍Đ��t���[�����I�����Ă����玟�̃A�j���[�V�����Ɉڍs����
	else if (this->useAnimationType == static_cast<short>(UseAnimationType::SMASH))
	{
		this->totalPlayTime += playTime;
		if (this->smashCancelPlayTime <= this->totalPlayTime)
		{
			this->useAnimationType++;
			this->totalPlayTime = 0.0f;
		}
		return ActionNode::NodeState::RUNNING;
	}
	//�A�j���[�V�������I�����Ă�����
	else if (enemy.GetIsChangeAnimation())
	{
		//�@�����A�j���[�V�������I�����Ă����琬����Ԃ�
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->useAnimationType = 0;
			//�A�N�V�����̉���
			rootNode.ExitCurrentBattleAction();
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�Ȃ�
		this->useAnimationType++;
		return ActionNode::NodeState::RUNNING;
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
