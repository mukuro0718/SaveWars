#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_RaiseLevel.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_RaiseLevel::Beast_RaiseLevel()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_RaiseLevel::~Beast_RaiseLevel()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_RaiseLevel::NodeState Beast_RaiseLevel::Update()
{
	/*�A�j���[�V����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		int animationType = static_cast<int>(Beast::AnimationType::RAISE_LEVEL);
		enemy.SetNowAnimation(animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//�A�j���[�V�����̍Đ�
		enemy.PlayAnimation();
	}

	/*��Ԃ�Ԃ�*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (enemy.GetIsChangeAnimation())
		{
			auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
			rootNode.RaiseLevel();
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�͎��s����Ԃ�
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
