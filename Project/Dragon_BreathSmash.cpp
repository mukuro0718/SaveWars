#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_BreathSmash.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_BreathSmash::Dragon_BreathSmash()
	: useAnimationType(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::WALK));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::BREATH));
	this->animationType.emplace_back(static_cast<int>(Dragon::AnimationType::SMASH));
	
	for (int i = 0; i < this->animationType.size(); i++)
	{
		this->animationPlayTime.emplace_back(json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType[i]]);
	}
	this->maxUseAnimation	= static_cast<int>(UseAnimationType::SMASH);
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
	/*���x���O�ȏ�܂��͍ŏ��ɂ��̃A�N�V�����ɂȂ������ړ��������s��*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());

	if (this->useAnimationType == 1)
	{
		enemy.UpdateSpeed(0.0f, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}
	else
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
	}

	/*�A�N�V�����̏�Ԃ��Z�b�g*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.GetCurrentAction() != this->actionType)
	{
		rootNode.SetCurrentAction(this->actionType);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation(this->animationType[this->useAnimationType], this->animationPlayTime[this->useAnimationType]);

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		//�@�����A�j���[�V�������I�����Ă����琬����Ԃ�
		if (this->useAnimationType == static_cast<short>(UseAnimationType::SMASH))
		{
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�Ȃ�
		else
		{
			//�ڕW�܂ł̋������ߐڍU���͈͂Ȃ�
			if (rootNode.GetToTargetDistance() >= this->nearAttackRange)
			{
				this->useAnimationType = static_cast<short>(UseAnimationType::WALK);
			}
			else
			{
				this->useAnimationType = static_cast<short>(UseAnimationType::SMASH);
			}
			return ActionNode::NodeState::RUNNING;
		}
	}
	//����ȊO�͎��s����Ԃ�
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
