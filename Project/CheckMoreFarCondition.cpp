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
CheckMoreFarCondition::CheckMoreFarCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckMoreFarCondition::~CheckMoreFarCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckMoreFarCondition::NodeState CheckMoreFarCondition::Update()
{
	/*��Ԃ�NORMAL�o�Ȃ����FAILURE��Ԃ�*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�ړ��ڕW�܂ł̋��������߂�*/
	float distance = rootNode.GetToTargetDistance();

	/*�������������U���͈͓̔��������玸�s*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (distance < json.GetJson(JsonManager::FileType::BEAST)["APPROACH_DISTANCE"]) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}