#include "UseSTL.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CheckDyingCondition::CheckDyingCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckDyingCondition::~CheckDyingCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckDyingCondition::NodeState CheckDyingCondition::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�_�E���l���萔�ȏ�ɂȂ����琬����Ԃ�*/
	if (enemy.GetHP() <= 0)
	{
		return NodeState::SUCCESS;
	}
	return NodeState::FAILURE;
}
