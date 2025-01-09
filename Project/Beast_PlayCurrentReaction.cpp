#include <DxLib.h>
#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_PlayCurrentReaction.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_PlayCurrentReaction::Beast_PlayCurrentReaction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_PlayCurrentReaction::~Beast_PlayCurrentReaction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_PlayCurrentReaction::NodeState Beast_PlayCurrentReaction::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	return rootNode.GetReaction().Update();
}
