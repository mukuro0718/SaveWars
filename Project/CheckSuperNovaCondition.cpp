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
CheckSuperNovaCondition::CheckSuperNovaCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckSuperNovaCondition::~CheckSuperNovaCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckSuperNovaCondition::NodeState CheckSuperNovaCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*���x�����ő傶��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetLevel() != static_cast<int>(BeastBehaviorTree::LevelStage::Lv4)) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}