#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_CalcToTargetDistance.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_CalcToTargetDistance::Beast_CalcToTargetDistance()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_CalcToTargetDistance::~Beast_CalcToTargetDistance()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_CalcToTargetDistance::NodeState Beast_CalcToTargetDistance::Update()
{
	auto& tree = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	
	/*���������߂�*/
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(),enemy.GetRigidbody().GetPosition());
	float distance = VSize(toTarget);
	tree.SetToTargetDistance(distance);
	
	/*��Ԃ�Ԃ�*/
	{
		return ActionNode::NodeState::SUCCESS;
	}
}
