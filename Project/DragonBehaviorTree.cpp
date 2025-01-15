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
/// コンストラクタ
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
	: Selector_DyingOrBattleOrIdle	 (nullptr)
	, debugActionNode				 (nullptr)
	, currentBattleAction			 (nullptr)
	, state							 (DragonState::NORMAL)
	, prevNodeState					 (BehaviorTreeNode::NodeState::NONE_ACTIVE)
	, toTargetDistance				 (0.0f)
	, innerProductOfDirectionToTarget(0.0f)
	, currentAction					 (0)
	, stamina						 (0)
	, isActive						 (true)
	, isSelectedBattleAction		 (false)
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*大元のツリーの作成*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*デスノード*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanZero());
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());

	/*バトルアクション*/
	////ブレス＋叩きつけ
	//BehaviorTreeNode* Sequencer_BreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH)));
	//Sequencer_BreathSmashIfIntervalIsOver->AddChild(*new Dragon_BreathSmash());
	////叩きつけ＋回転攻撃
	//BehaviorTreeNode* Sequencer_SmashRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_SmashRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SMASH_ROTATE)));
	//Sequencer_SmashRotateIfIntervalIsOver->AddChild(*new Dragon_SmashRotate());
	////なぎ払い＋叩きつけ
	//BehaviorTreeNode* Sequencer_SweepSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_SMASH)));
	//Sequencer_SweepSmashIfIntervalIsOver->AddChild(*new Dragon_SweepSmash());
	////回転攻撃＋なぎ払い
	//BehaviorTreeNode* Sequencer_RotateSweepIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP)));
	//Sequencer_RotateSweepIfIntervalIsOver->AddChild(*new Dragon_RotateSweep());
	////なぎ払い＋ブレス
	//BehaviorTreeNode* Sequencer_SweepBreathIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_BREATH)));
	//Sequencer_SweepBreathIfIntervalIsOver->AddChild(*new Dragon_SweepBreath());
	////ブレス＋叩きつけ＋回転攻撃
	//BehaviorTreeNode* Sequencer_BreathSmashRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH_ROTATE)));
	//Sequencer_BreathSmashRotateIfIntervalIsOver->AddChild(*new Dragon_BreathSmashRotate());
	////回転攻撃＋なぎ払い＋叩きつけ
	//BehaviorTreeNode* Sequencer_RotateSweepSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP_SMASH)));
	//Sequencer_RotateSweepSmashIfIntervalIsOver->AddChild(*new Dragon_RotateSweepSmash());
	////なぎ払い＋ブレス＋叩きつけ
	//BehaviorTreeNode* Sequencer_SweepBreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_BREATH_SMASH)));
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_SweepBreathSmash());
	////ブレス＋叩きつけ＋なぎ払い＋回転攻撃
	//BehaviorTreeNode* Sequencer_BreathSmashSweepRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashSweepRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH_SWEEP_ROTATE)));
	//Sequencer_BreathSmashSweepRotateIfIntervalIsOver->AddChild(*new Dragon_BreathSmashSweepRotate());
	////回転攻撃＋なぎ払い＋ブレス＋叩きつけ
	//BehaviorTreeNode* Sequencer_RotateSweepBreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP_BREATH_SMASH)));
	//Sequencer_RotateSweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_RotateSweepBreathSmash());
	////なぎ払い＋叩きつけ＋回転攻撃＋ブレス
	//BehaviorTreeNode* Sequencer_SweepSmashRotateBreathIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_SMASH_ROTATE_BREATH)));
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_SweepSmashRotateBreath());
	//1段階目の攻撃アクション
	BehaviorTreeNode* Selector_OneStepAttackAction = new RandomSelector();
	Selector_OneStepAttackAction->AddChild(*new Dragon_BreathSmash());	//ブレス＋叩きつけ
	Selector_OneStepAttackAction->AddChild(*new Dragon_SmashRotate());	//叩きつけ＋回転攻撃
	Selector_OneStepAttackAction->AddChild(*new Dragon_SweepSmash());	//なぎ払い＋叩きつけ
	Selector_OneStepAttackAction->AddChild(*new Dragon_RotateSweep());	//回転攻撃＋なぎ払い
	Selector_OneStepAttackAction->AddChild(*new Dragon_SweepBreath());	//なぎ払い＋ブレス
	//2段階目の攻撃アクション
	BehaviorTreeNode* Selector_TwoStepAttackAction = new RandomSelector();
	Selector_TwoStepAttackAction->AddChild(*new Dragon_BreathSmashRotate());	//ブレス＋叩きつけ＋回転攻撃
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SmashRotate());	//叩きつけ＋回転攻撃
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SweepSmash());	//なぎ払い＋叩きつけ
	Selector_TwoStepAttackAction->AddChild(*new Dragon_RotateSweepSmash());	//回転攻撃＋なぎ払い＋叩きつけ
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SweepBreathSmash());	//なぎ払い＋ブレス＋叩きつけ
	//3段階目の攻撃アクション
	BehaviorTreeNode* Selector_ThreeStepAttackAction = new RandomSelector();
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_BreathSmashSweepRotate());//ブレス＋叩きつけ＋なぎ払い＋回転攻撃
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SmashRotate());//叩きつけ＋回転攻撃
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SweepSmashRotateBreath());//なぎ払い＋叩きつけ＋回転攻撃＋ブレス
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_RotateSweepBreathSmash());//回転攻撃＋なぎ払い＋ブレス＋叩きつけ
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SweepBreathSmash());//なぎ払い＋ブレス＋叩きつけ
	//段階ごとの攻撃アクション
	BehaviorTreeNode* Selector_StepByStepAttackAction = new SelectorNode();
	Selector_StepByStepAttackAction->AddChild(*Selector_OneStepAttackAction);
	Selector_StepByStepAttackAction->AddChild(*Selector_TwoStepAttackAction);
	Selector_StepByStepAttackAction->AddChild(*Selector_ThreeStepAttackAction);
	//アクティブフラグが立っているときのアクション
	BehaviorTreeNode* Sequencer_ActiveActionIfActiveFlagIsTrue = new SequencerNode();
	Sequencer_ActiveActionIfActiveFlagIsTrue->AddChild(*new Condition_IsDragonActiveFlagIsTrue());
	Sequencer_ActiveActionIfActiveFlagIsTrue->AddChild(*Selector_StepByStepAttackAction);
	//アクションが選択されているか
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedDragonBattleAction());
	Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	//アクティブかどうかを判断しアクションを決めるセレクターノード
	BehaviorTreeNode* Selector_ActiveOrNoneActiveAction = new SelectorNode();
	Selector_ActiveOrNoneActiveAction->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	Selector_ActiveOrNoneActiveAction->AddChild(*Sequencer_ActiveActionIfActiveFlagIsTrue);
	Selector_ActiveOrNoneActiveAction->AddChild(*Selector_OneStepAttackAction);

	/*ルートノードに各アクションツリーを挿入*/
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Selector_ActiveOrNoneActiveAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*new Dragon_Idle());

	this->debugActionNode = new Dragon_RotateSweepBreathSmash();

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{

}

/// <summary>
/// 初期化
/// </summary>
void DragonBehaviorTree::Initialize()
{
	//auto& json	= Singleton<JsonManager>::GetInstance();
	this->state		= DragonState::NORMAL;
}

/// <summary>
/// メンバ変数の更新
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*インターバルの計算*/
	for (auto interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*目標までの距離を求める*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*ボスの向いている方向とプレイヤーの位置との内積を取る*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// 更新
/// </summary>
void DragonBehaviorTree::Update()
{
	/*メンバ変数の更新*/
	UpdateMemberVariables();

	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->currentAction);

	/*ツリーの実行*/
	this->prevNodeState = this->Selector_DyingOrBattleOrIdle->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// 描画
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	this->debugActionNode->Draw();
#endif // _DEBUG

}

/// <summary>
/// バトルアクションの登録
/// </summary>
void DragonBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true;
}

/// <summary>
/// バトルアクションの解除
/// </summary>
void DragonBehaviorTree::ExitCurrentBattleAction()
{
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false;
}