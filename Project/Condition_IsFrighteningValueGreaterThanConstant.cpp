#include <DxLib.h>
#include "UseSTL.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsFrighteningValueGreaterThanConstant::Condition_IsFrighteningValueGreaterThanConstant(const int _specifiedFrighteningValue)
	: ConditionNode()
	, SPECIFIED_FRIGHTENING_VALUE(_specifiedFrighteningValue)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsFrighteningValueGreaterThanConstant::~Condition_IsFrighteningValueGreaterThanConstant()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsFrighteningValueGreaterThanConstant::NodeState Condition_IsFrighteningValueGreaterThanConstant::Update()
{
	/*�_�E���l���O�ȏ�Ȃ琬����Ԃ�*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetDamage() >= this->SPECIFIED_FRIGHTENING_VALUE)
	{
		rootNode.SetDamage(0);
		//printfDx("CONDITION_FRIGHT_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_FRIGHT_FAILURE\n");
	return NodeState::FAILURE;
}
