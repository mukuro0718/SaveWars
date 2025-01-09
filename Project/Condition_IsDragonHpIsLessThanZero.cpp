#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDragonHpIsLessThanZero::Condition_IsDragonHpIsLessThanZero()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonHpIsLessThanZero::~Condition_IsDragonHpIsLessThanZero()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonHpIsLessThanZero::NodeState Condition_IsDragonHpIsLessThanZero::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HP���O�ȉ��Ȃ�SUCCESS��Ԃ�*/
	if (enemy.GetHP() <= 0)
	{
		printfDx("CONDITION_HP_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	printfDx("CONDITION_HP_FAILURE\n");
	return NodeState::FAILURE;
}