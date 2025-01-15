#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_BreathSmashRotate.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_BreathSmashRotate::Dragon_BreathSmashRotate()
	: useAnimationType	 (0)
	, maxUseAnimation	 (0)
	, nearAttackRange	 (0.0f)
	, smashCancelPlayTime(0.0f)
	, totalPlayTime		 (0.0f)
{
	/*�g�p����A�j���[�V����*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::ROTATE_ATTACK));

	/*�g�p����A�j���[�V�����̍Đ�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_SMASH_ROTATE_PLAY_TIME"];
	this->animationPlayTime		= playTime;
	this->maxUseAnimation		= static_cast<int>(UseAnimationType::ROTATE);
	this->actionType			= static_cast<short>(DragonBehaviorTree::ActionType::BREATH_SMASH_ROTATE);
	this->maxSpeed				= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel					= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel					= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->nearAttackRange		= json.GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"];
	this->smashCancelPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["SMASH_CANCEL_PLAY_TIME"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_BreathSmashRotate::~Dragon_BreathSmashRotate()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_BreathSmashRotate::NodeState Dragon_BreathSmashRotate::Update()
{
	/*�ړ�����*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	short walkIndex = static_cast<short>(UseAnimationType::WALK);
	if (this->useAnimationType == walkIndex)
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
	if (this->useAnimationType == walkIndex)
	{
		if (rootNode.GetToTargetDistance() <= this->nearAttackRange)
		{
			this->useAnimationType++;
		}
	}
	//�@�����U�����Ɏw��̍Đ��t���[�����I�����Ă����玟�̃A�j���[�V�����Ɉڍs����
	else if (this->useAnimationType == static_cast<short>(UseAnimationType::SMASH))
	{
		this->totalPlayTime += playTime;
		if (this->smashCancelPlayTime <= this->totalPlayTime)
		{
			this->useAnimationType++;
		}
	}
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//��]�U���A�j���[�V�������I�����Ă����琬����Ԃ�
		if (this->useAnimationType == this->maxUseAnimation)
		{
			this->totalPlayTime = 0;
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
