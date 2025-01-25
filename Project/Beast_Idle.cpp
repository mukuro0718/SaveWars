#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_Idle.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_Idle::Beast_Idle()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::IDLE);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(BeastBehaviorTree::ActionType::IDLE);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_Idle::~Beast_Idle()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_Idle::NodeState Beast_Idle::Update()
{
	/*アニメーション*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());
	/*アクションの状態をセット*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		rootNode.SetSelectAction(this->actionType);
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		rootNode.EntryCurrentBattleAction(*this);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation();

	/*移動*/
	if (enemy.GetSpeed() != 0.0f)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);

	}

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
		//アクションの解除
		rootNode.ExitCurrentBattleAction();
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
