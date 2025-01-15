#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackData.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Dragon_BreathSmash.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"
#include "ReactionType.h"
#include "HitStop.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_BreathSmash::Dragon_BreathSmash()
	: MAX_USE_ANIMATION	(static_cast<int>(UseAnimationType::SMASH))
	, NEAR_ATTACK_RANGE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"])
	, useAnimationType(0)
	, SWEEP_CANCEL_PLAY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["NEAR_ATTACK_RANGE"])
{
	/*�g�p����A�j���[�V����*/
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	
	/*�����o�ϐ��̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<float> playTime = json.GetJson(JsonManager::FileType::DRAGON)["BREATH_SMASH_PLAY_TIME"];
	this->animationPlayTime = playTime;
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::BREATH_SMASH);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::DRAGON)["WALK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_BreathSmash::~Dragon_BreathSmash()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_BreathSmash::NodeState Dragon_BreathSmash::Update()
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
	enemy.PlayAnimation(this->animationType[this->useAnimationType], this->animationPlayTime[this->useAnimationType]);

	/*��Ԃ�Ԃ�*/
	//�ړ���Ԃ��ߐڍU���͈͂Ȃ玟�̃A�j���[�V�����Ɉڍs����
	if (this->useAnimationType == static_cast<short>(UseAnimationType::WALK))
	{
		if (rootNode.GetToTargetDistance() <= this->NEAR_ATTACK_RANGE)
		{
			this->useAnimationType++;
		}
	}
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�@�����A�j���[�V�������I�����Ă����琬����Ԃ�
		if (this->useAnimationType == this->MAX_USE_ANIMATION)
		{
			//�A�N�V�����̉���
			rootNode.ExitCurrentBattleAction();
			this->useAnimationType = 0;
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
