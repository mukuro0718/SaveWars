#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Down.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Down::Beast_Down()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_Down::~Beast_Down()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_Down::NodeState Beast_Down::Update()
{
	/*�A�j���[�V����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		int animationType = static_cast<int>(Beast::AnimationType::DOWN);
		enemy.SetNowAnimation(animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//�A�j���[�V�����̍Đ�
		enemy.PlayAnimation();
	}

	/*��Ԃ�Ԃ�*/
	{
		if (enemy.GetIsChangeAnimation())
		{
			return ActionNode::NodeState::SUCCESS;
		}
	}
	return ActionNode::NodeState::RUNNING;
}
