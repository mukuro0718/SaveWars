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
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_Down::Beast_Down()
	: isInitialize(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::DOWN);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::DOWN);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];

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
		//������Root�ɓo�^
		rootNode.EntryCurrentReaction(*this);
	}

	/*�A�j���[�V�����̍Đ�*/
	enemy.PlayAnimation();

	/*�ړ�*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*��Ԃ�Ԃ�*/
	if (enemy.GetIsChangeAnimation())
	{
		//�C���^�[�o���̐ݒ�
		rootNode.SetInterval(this->actionType, this->interval);
		//�o�^������
		rootNode.ExitCurrentReaction();
		return ActionNode::NodeState::SUCCESS;
	}
	return ActionNode::NodeState::RUNNING;
}
