#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CheckDownCondition::CheckDownCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckDownCondition::~CheckDownCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckDownCondition::NodeState CheckDownCondition::Update()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�_�E���l���萔�ȏ�ɂȂ����琬����Ԃ�*/
	if (rootNode.GetDownValue() >= json.GetJson(JsonManager::FileType::BEAST)["MAX_DOWN_VALUE"])
	{
		return NodeState::SUCCESS;
	}
	return NodeState::FAILURE;
}
