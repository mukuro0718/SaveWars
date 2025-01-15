#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_PlayCurrentBattleAction.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_PlayCurrentBattleAction::Dragon_PlayCurrentBattleAction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_PlayCurrentBattleAction::~Dragon_PlayCurrentBattleAction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_PlayCurrentBattleAction::NodeState Dragon_PlayCurrentBattleAction::Update()
{
	//�A�N�V�����̏�Ԃ��Z�b�g
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	return rootNode.GetBattleAction().Update();
}
