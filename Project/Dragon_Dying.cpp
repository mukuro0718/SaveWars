#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_Dying.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_Dying::Dragon_Dying()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Dragon::AnimationType::DYING);
	this->animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_PLAY_TIME"][this->animationType];
	this->actionType		= static_cast<short>(DragonBehaviorTree::ActionType::DYING);
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_Dying::~Dragon_Dying()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_Dying::NodeState Dragon_Dying::Update()
{
	/*速度が０以上または最初にこのアクションになった時移動処理を行う*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	int prevAction = rootNode.GetCurrentAction();
	if (enemy.GetSpeed() != 0.0f || prevAction != this->actionType)
	{
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*アクションの状態をセット*/
	if (prevAction != this->actionType)
	{
		rootNode.SetCurrentAction(this->actionType);
	}

	/*アニメーションの再生*/
	enemy.PlayAnimation(this->animationType, this->animationPlayTime);

	/*状態を返す*/
	//アニメーションが終了していたら
	if (enemy.GetIsChangeAnimation())
	{
		//インターバルの設定
		rootNode.SetInterval(this->actionType);
		enemy.OffIsAlive();
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	else
	{
		return ActionNode::NodeState::RUNNING;
	}
}
