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
Condition_IsNowLevelIsSameAsTheSpecifiedLevel::Condition_IsNowLevelIsSameAsTheSpecifiedLevel(const int _specifiedLevel)
	: SPECIFIED_LEVEL(_specifiedLevel)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsNowLevelIsSameAsTheSpecifiedLevel::~Condition_IsNowLevelIsSameAsTheSpecifiedLevel()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsNowLevelIsSameAsTheSpecifiedLevel::NodeState Condition_IsNowLevelIsSameAsTheSpecifiedLevel::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetLevel() != this->SPECIFIED_LEVEL)
	{
		printfDx("CONDITION_LEVEL_FAILURE\n");
		return NodeState::FAILURE;
	}
	printfDx("CONDITION_LEVEL_SUCCESS\n");
	return NodeState::SUCCESS;
}