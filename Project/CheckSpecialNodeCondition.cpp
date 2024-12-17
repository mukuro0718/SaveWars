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
CheckSpecialNodeCondition::CheckSpecialNodeCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckSpecialNodeCondition::~CheckSpecialNodeCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckSpecialNodeCondition::NodeState CheckSpecialNodeCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::RAISE_LEVEL)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}