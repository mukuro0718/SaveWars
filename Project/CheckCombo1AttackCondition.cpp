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
CheckCombo1AttackCondition::CheckCombo1AttackCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckCombo1AttackCondition::~CheckCombo1AttackCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckCombo1AttackCondition::NodeState CheckCombo1AttackCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::COMBO_ATTACK_1)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}