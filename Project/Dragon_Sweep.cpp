#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_Sweep.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_Sweep::Dragon_Sweep()
	: USE_COLLIDER_INDEX	(static_cast<short>(Dragon::AttackCollider::SWEEP))
	, FIX_ROTATE_FRAME		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["FIX_ROTATE_FRAME"])
	, SWEEP_CANCEL_PLAY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::DRAGON)["SWEEP_CANCEL_PLAY_TIME"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType				= static_cast<int>(Dragon::AnimationType::SWEEP);
	vector<float>animationPlayTime	= json.GetJson(JsonManager::FileType::DRAGON)["SWEEP_PLAY_TIME"];
	this->animationPlayTime			= animationPlayTime;
	this->actionType				= static_cast<short>(DragonBehaviorTree::ActionType::SWEEP);
	this->maxSpeed					= 0.0f;
	this->accel						= json.GetJson(JsonManager::FileType::DRAGON)["ACCEL"];
	this->decel						= json.GetJson(JsonManager::FileType::DRAGON)["DECEL"];
	this->isFixRotate				= true;
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_Sweep::~Dragon_Sweep()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_Sweep::NodeState Dragon_Sweep::Update()
{
	/*アクションの状態をセット*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (this->frameCount == 0)
	{
		//アクションの設定
		rootNode.SetCurrentAction(this->actionType);
		//アクションの登録
		rootNode.EntryCurrentBattleAction(*this);
		//段階を取得
		this->currentDragonStage = rootNode.GetDragonStage();
		//段階が3段階の時に、アクションキャンセルフラグが立っていなければ立てる
		if (rootNode.GetDragonStage() == static_cast<short>(DragonBehaviorTree::DragonStage::RAMPAGE) &&
			!rootNode.GetIsCancelAction())
		{
			rootNode.OnIsCancelAction();
		}
	}

	/*フレームカウントの増加*/
	if (this->isFixRotate)
	{
		this->frameCount++;
		if (this->frameCount >= this->FIX_ROTATE_FRAME)
		{
			this->isFixRotate = false;
		}
	}

	/*移動*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Dragon&>(enemyManager.GetCharacter());
	if (this->isFixRotate)
	{
		enemy.Move(this->maxSpeed, this->accel, this->decel, false);
		return ActionNode::NodeState::SUCCESS;
	}
	else
	{
		enemy.UpdateSpeed(0.0f, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*当たり判定コライダーの更新*/
	enemy.UpdateAttackCollider(this->USE_COLLIDER_INDEX, this->nowTotalPlayTime);

	/*アニメーションの再生*/
	float playTime = this->animationPlayTime[this->currentDragonStage];
	this->nowTotalPlayTime += playTime;
	enemy.PlayAnimation(this->animationType, playTime);

	/*状態を返す*/
	if (rootNode.GetIsCancelAction())
	{
		if (this->SWEEP_CANCEL_PLAY_TIME <= this->nowTotalPlayTime)
		{
			this->isFixRotate = true;
			this->frameCount = 0;
			this->nowTotalPlayTime = 0.0f;
			enemy.OffAttackCollider(this->USE_COLLIDER_INDEX);
			rootNode.ExitCurrentBattleAction();
			rootNode.CalcAttackCount();
			return ActionNode::NodeState::SUCCESS;
		}
	}
	//アニメーションが終了していたら
	else if (enemy.GetIsChangeAnimation())
	{
		this->isFixRotate = true;
		this->frameCount = 0;
		this->nowTotalPlayTime = 0.0f;
		enemy.OffAttackCollider(this->USE_COLLIDER_INDEX);
		rootNode.ExitCurrentBattleAction();
		rootNode.CalcAttackCount();
		return ActionNode::NodeState::SUCCESS;
	}
	//それ以外は実行中を返す
	return ActionNode::NodeState::RUNNING;
}
