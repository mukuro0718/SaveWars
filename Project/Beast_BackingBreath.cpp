#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_BackingBreath.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_BackingBreath::Beast_BackingBreath()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_BackingBreath::~Beast_BackingBreath()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_BackingBreath::NodeState Beast_BackingBreath::Update()
{
	/*�A�j���[�V����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		int animationType = static_cast<int>(Beast::AnimationType::BACKING_BREATH);
		enemy.SetNowAnimation(animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//�A�j���[�V�����̍Đ�
		enemy.PlayAnimation();
	}

	/*�ړ�*/
	float maxSpeed = json.GetJson(JsonManager::FileType::BEAST)["BACKING_BREATH_SPEED"];
	float accel = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	float decel = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	//�ړ����x�̍X�V
	enemy.UpdateSpeed(maxSpeed, decel, accel);
	//�ړ��x�N�g�����o��
	enemy.UpdateVelocity(false);

	/*��Ԃ�Ԃ�*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (enemy.GetIsChangeAnimation())
		{
			auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
			rootNode.SetBeastState(BeastBehaviorTree::BeastState::ANGRY);
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�͎��s����Ԃ�
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
