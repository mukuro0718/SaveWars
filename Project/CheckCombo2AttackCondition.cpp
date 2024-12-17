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
CheckCombo2AttackCondition::CheckCombo2AttackCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckCombo2AttackCondition::~CheckCombo2AttackCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckCombo2AttackCondition::NodeState CheckCombo2AttackCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::RIGHT_FOOT_ATTACK)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}