#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsNodeStateIsSameAsSpecifiedLevel::Condition_IsNodeStateIsSameAsSpecifiedLevel(const BehaviorTreeNode::NodeState _specifiedState)
	: SPECIFIED_STATE(_specifiedState)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsNodeStateIsSameAsSpecifiedLevel::~Condition_IsNodeStateIsSameAsSpecifiedLevel()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsNodeStateIsSameAsSpecifiedLevel::NodeState Condition_IsNodeStateIsSameAsSpecifiedLevel::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetNodeState() == this->SPECIFIED_STATE)
	{
		printfDx("CONDITION_NODESTATE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	printfDx("CONDITION_NODESTATE_FAILURE\n");
	return NodeState::FAILURE;
}