#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CheckAngryCondition::CheckAngryCondition()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CheckAngryCondition::~CheckAngryCondition()
{

}

/// <summary>
/// �X�V
/// </summary>
CheckAngryCondition::NodeState CheckAngryCondition::Update()
{
	/*��Ԃ�NORMAL�o�Ȃ����FAILURE��Ԃ�*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetBeastState() != BeastBehaviorTree::BeastState::NORMAL) return NodeState::FAILURE;

	/*�_���[�W�̒~��*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	int nowDamage = rootNode.GetDamage();
	int prevHp = rootNode.GetPrevHp();
	int nowHP = enemy.GetHP();
	int newDamage = nowDamage + prevHp - nowHP;
	rootNode.SetDamage(newDamage);
	rootNode.SetPrevHp(nowHP);

	/*�~�ς����_���[�W���萔�ȏ�ɂȂ����琬����Ԃ�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (newDamage >= json.GetJson(JsonManager::FileType::BEAST)["CHANGE_ANGRY_LIMIT_DAMAGE"])
	{
		return NodeState::SUCCESS;
	}
	return NodeState::FAILURE;
}
