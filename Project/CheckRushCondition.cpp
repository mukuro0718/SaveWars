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
CheckRushCondition::CheckRushCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckRushCondition::~CheckRushCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckRushCondition::NodeState CheckRushCondition::Update()
{
	/*��Ԃ�NORMAL�o�Ȃ����FAILURE��Ԃ�*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�ړ��ڕW�܂ł̋��������߂�*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Enemy&>(enemyManager.GetCharacter());
	float distance = VSize(VSub(enemy.GetMoveTarget(), enemy.GetRigidbody().GetPosition()));

	/*������rey�͈͓̔��������玸�s*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (rootNode.GetBeastState() != BeastBehaviorTree::BeastState::ANGRY) return NodeState::FAILURE;
	if (distance < json.GetJson(JsonManager::FileType::BEAST)["RAY_RANGE"]) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}