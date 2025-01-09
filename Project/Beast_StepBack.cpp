#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_StepBack.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_StepBack::Beast_StepBack()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::STEP_BACK);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::STEP_BACK);
	this->maxSpeed			= json.GetJson(JsonManager::FileType::BEAST)["STEP_BACK_SPEED"];
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_StepBack::~Beast_StepBack()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_StepBack::NodeState Beast_StepBack::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());

	/*�o�^����Ă���A�N�V�����Ǝ��ۂ̃A�N�V�������قȂ��Ă�����*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//�A�j���[�V�����̎�ނ�ݒ�
		enemy.SetNowAnimation(this->animationType);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//�A�N�V�����̐ݒ�
		rootNode.SetSelectAction(this->actionType);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	enemy.Move(this->maxSpeed, this->accel, this->decel, false);

	/*��Ԃ�Ԃ�*/
	{
		//�A�j���[�V�������I�����Ă�����
		if (enemy.GetIsChangeAnimation())
		{
			//�C���^�[�o���̐ݒ�
			rootNode.SetInterval(this->actionType, this->interval);
			return ActionNode::NodeState::SUCCESS;
		}
		//����ȊO�͎��s����Ԃ�
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
