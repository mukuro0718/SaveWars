#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
	: Selector_DyingOrBattleOrIdle	(nullptr)
	, debugActionNode				(nullptr)
	, state							(DragonState::NORMAL)
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�匳�̃c���[�̍쐬*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*�f�X�m�[�h*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanZero());
	Sequencer_DyingIfHpIsLessThanZero->AddChild();

	/*�o�g���m�[�h*/

	/*�ҋ@�m�[�h*/

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{

}

/// <summary>
/// ������
/// </summary>
void DragonBehaviorTree::Initialize()
{
	//auto& json	= Singleton<JsonManager>::GetInstance();
	this->state		= DragonState::NORMAL;
}

/// <summary>
/// �����o�ϐ��̍X�V
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*�C���^�[�o���̌v�Z*/
	for (auto interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*�ڕW�܂ł̋��������߂�*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*�{�X�̌����Ă�������ƃv���C���[�̈ʒu�Ƃ̓��ς����*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// �X�V
/// </summary>
void DragonBehaviorTree::Update()
{
	/*�����o�ϐ��̍X�V*/
	UpdateMemberVariables();

	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->currentAction);

	/*�c���[�̎��s*/
	this->prevNodeState = this->Selector_DeathOrReactionOrBattleOrBreak->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// �`��
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	this->debugActionNode->Draw();
#endif // _DEBUG

}

