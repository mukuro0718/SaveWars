#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Rush.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Rush::Beast_Rush()
{
	this->animationSet.emplace(AnimationStage::START, static_cast<int>(Beast::AnimationType::RUSH_START));
	this->animationSet.emplace(AnimationStage::LOOP, static_cast<int>(Beast::AnimationType::RUSH_LOOP));
	this->animationSet.emplace(AnimationStage::NORMAL_END, static_cast<int>(Beast::AnimationType::RUSH_NORMAL_END));
	this->animationSet.emplace(AnimationStage::ANGRY_END, static_cast<int>(Beast::AnimationType::RUSH_ROTATE_END));
	this->nextStageSet.emplace(AnimationStage::START, AnimationStage::LOOP);
	this->nextStageSet.emplace(AnimationStage::LOOP, AnimationStage::NORMAL_END);
	this->nextStageSet.emplace(AnimationStage::NORMAL_END, AnimationStage::START);
	this->nextStageSet.emplace(AnimationStage::ANGRY_END, AnimationStage::START);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Rush::~Beast_Rush()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Rush::NodeState Beast_Rush::Update()
{
	/*�A�j���[�V����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		int animationType = this->animationSet[this->stage];
		enemy.SetNowAnimation(animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//�A�j���[�V�����̍Đ�
		enemy.PlayAnimation();
	}

	/*��Ԃ�Ԃ�*/
	//�A�j���[�V�������I�����Ă�����
	if (enemy.GetIsChangeAnimation())
	{
		auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
		this->stage = this->nextStageSet[this->stage];
		if (this->stage == AnimationStage::NORMAL_END && 
			rootNode.GetBeastState() == BeastBehaviorTree::BeastState::ANGRY)
		{
			this->stage = AnimationStage::ANGRY_END;
		}
		if (this->stage == AnimationStage::START)
		{
			return ActionNode::NodeState::SUCCESS;
		}
		return ActionNode::NodeState::RUNNING;
	}
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
