#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_RaiseLevel.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "EnemyManager.h"
#include "BeastBehaviorTree.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_RaiseLevel::Beast_RaiseLevel()
	: effectStartCount	(false)
	, frameCount		(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->animationType		= static_cast<int>(Beast::AnimationType::RAISE_LEVEL);
	this->actionType		= static_cast<int>(BeastBehaviorTree::ActionType::RAISE_LEVEL);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->animationType];
	this->interval			= json.GetJson(JsonManager::FileType::BEAST)["ACTION_INTERVAL"][this->actionType];
	this->maxSpeed			= 0.0f;
	this->accel				= json.GetJson(JsonManager::FileType::BEAST)["ACCEL"];
	this->decel				= json.GetJson(JsonManager::FileType::BEAST)["DECEL"];
	this->effectStartCount	= json.GetJson(JsonManager::FileType::BEAST)["RAISE_LEVEL_EFFECT_START_COUNT"];
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_RaiseLevel::~Beast_RaiseLevel()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_RaiseLevel::NodeState Beast_RaiseLevel::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Beast&>(enemyManager.GetCharacter());

	/*登録されているアクションと実際のアクションが異なっていたら*/
	if (rootNode.GetNowSelectAction() != this->actionType)
	{
		//アニメーションの種類を設定
		enemy.SetNowAnimation(this->animationType);
		//アニメーション再生時間の設定
		enemy.SetAnimationPlayTime(this->animationPlayTime);
		//アクションの設定
		rootNode.SetSelectAction(this->actionType);
		//自分をRootに登録
		rootNode.EntryCurrentBattleAction(*this);
		enemy.UpdateSpeed(this->maxSpeed, this->accel, this->decel);
		enemy.UpdateVelocity(false);
	}

	/*エフェクトの制御*/
	if (this->frameCount < this->effectStartCount)
	{
		this->frameCount++;
		if (this->frameCount == this->effectStartCount)
		{
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BEAST_RAISE_LEVEL);
		}
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
	{
		//アニメーションが終了していたら
		if (enemy.GetIsChangeAnimation())
		{
			//インターバルの設定
			rootNode.SetInterval(this->actionType, this->interval);
			//登録を解除
			rootNode.ExitCurrentBattleAction();
			//レベルの上昇
			rootNode.RaiseLevel();
			this->frameCount = 0;
			return ActionNode::NodeState::SUCCESS;
		}
		//それ以外は実行中を返す
		else
		{
			return ActionNode::NodeState::RUNNING;
		}
	}
}
