#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_TurnLeft.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_TurnLeft::Beast_TurnLeft()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_TurnLeft::~Beast_TurnLeft()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_TurnLeft::NodeState Beast_TurnLeft::Update()
{
	/*�A�j���[�V����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		int animationType = static_cast<int>(Beast::AnimationType::WALK);
		enemy.SetNowAnimation(animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][animationType]);
		//�A�j���[�V�����̍Đ�
		enemy.PlayAnimation();
	}

	/*�ړ�*/
	float maxSpeed = json.GetJson(JsonManager::FileType::BEAST)["WALK_SPEED"];
	float accel = json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	float decel = json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	enemy.Move(maxSpeed, accel, decel, false);

	/*��Ԃ�Ԃ�*/
	{
		return ActionNode::NodeState::SUCCESS;
	}
}
