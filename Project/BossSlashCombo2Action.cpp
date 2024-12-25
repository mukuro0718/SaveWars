#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "VECTORtoUseful.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlashCombo2Attack.h"
#include "BossSlashCombo2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossSlashCombo2Action::BossSlashCombo2Action()
	:isClose(false)
{
	auto& json	= Singleton<JsonManager>::GetInstance();
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::SLASH_COMBO_2);
	this->attack			= new BossSlashCombo2Attack(ATTACK_TYPE);
	this->targetOffset		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_TARGET_OFFSET"];
	this->hitStopTime		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE];
	this->hitStopType		= static_cast<int>(HitStop::Type::STOP);
	this->hitStopDelay		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE];
	this->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE];
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->rotateFixFrame	= json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"];
	this->moveFrame			= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_MOVE_FRAME"];
	this->rotateFrame1		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ROTATE_FRAME"];
	this->rotateFrame2		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ROTATE_FRAME_2"];
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_MOVE_SPEED"];
	this->stopDistance		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_STOP_MOVE_DISTANCE"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_COMBO_2);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_INTERVAL"];
	this->checkState		= static_cast<int>(Boss::BossState::ANGRY);
	this->actionDistance	= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][ATTACK_TYPE];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->normalDisireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
}

/// <summary>
/// デストラクタ
/// </summary>
BossSlashCombo2Action::~BossSlashCombo2Action()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossSlashCombo2Action::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossSlashCombo2Action::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();

	/*攻撃準備*/
	{
		//初期化されていなかったら
		if (!this->isInitialize)
		{
			//攻撃タイプの設定
			_boss.SetAttackType(Boss::AttackType::SLASH_COMBO_2);
			//攻撃フラグを立てる
			this->attack->OnIsStart();
			//移動目標の設定
			const VECTOR TARGET = player.GetRigidbody().GetPosition();
			const VECTOR POSITION = _boss.GetRigidbody().GetPosition();
			VECTOR positonToTargetVector = VSub(POSITION, TARGET);
			VECTOR targetOffset = VNorm(VCross(VNorm(positonToTargetVector), Gori::UP_VEC));
			this->moveTarget = VAdd(TARGET, VScale(targetOffset, this->targetOffset));
			//初期化フラグを立てる
			this->isInitialize = true;
		}
	}

	/*ヒットストップ*/
	{
		//攻撃が当たっていたら
		if (this->attack->GetIsHitAttack())
		{
			//攻撃の種類
			//ヒットストップの設定
			this->hitStop->SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
			//攻撃ヒットフラグを下す
			this->attack->OffIsHitAttack();
		}
		//ヒットストップをしていたら早期リターン
		if (this->hitStop->IsHitStop()) return;
	}


	/*カウントの計測*/
	this->frameCount++;

	/*移動処理*/
	{
		//使用する変数
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//座標
			  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//回転率
			  VECTOR toTarget	 = VSub(POSITION, this->moveTarget);	//座標と移動目標間のベクトル
			  VECTOR direction   = VGet(0.0f, 0.0f, 0.0f);				//向き
			  float  speed		 = 0.0f;								//移動スピードの設定
			  bool   isRotation  = false;								//回転するか
			  bool   isMove		 = false;								//移動するか
		{
			//フレームカウントが定数未満だったら許可フラグを立てる
			if (this->frameCount > this->rotateFixFrame)
			{
				this->isAllowAction = true;
			}
			//フレームカウントが定数以内なら移動フラグを立てる
			if (this->frameCount < this->moveFrame)
			{
				isMove = true;
			}
			//フレームカウントが定数以内なら回転フラグを立てる
			if (this->frameCount < this->rotateFrame1 || 
				this->frameCount >= this->rotateFrame2)
			{
				isRotation = true;
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
			////フレームカウントが定数以内なら移動目標を更新する
			//if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_HOMING_FRAME"] ||
			//	this->frameCount >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_ROTATE_FRAME_2"])
			//{
			//}
		}
		//回転処理
		{
			if (isRotation)
			{
				//回転率を補完する
				nowRotation = GetLerpRotation(_boss, toTarget, nowRotation, this->rotateLerpValue);
				//回転率を設定
				_boss.SetRotation(nowRotation);
			}
		}
		////アクション許可フラグが立っていなければ早期リターン
		//if (!this->isAllowAction)return;
		//移動速度の設定
		{
			//一度も一定距離に近づいていない && 移動フラグが立っていたら
			if (!this->isClose && isMove)
			{
				//座標と移動目標との距離を求める
				const float DISTANCE = VSize(toTarget);
				//距離が定数以上なら速度を設定する
				if (DISTANCE >= this->stopDistance)
				{
					speed = this->speed;
				}
				//一定未満ならフラグを立てる
				else
				{
					this->isClose = true;
				}
			}
		}
		//移動ベクトルの設定
		{
			//移動目標の設定
			_boss.SetNowMoveTarget(this->moveTarget);
			//新しい移動ベクトルの算出
			VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
			//移動ベクトルの設定
			_boss.SetVelocity(newVelocity);
		}
	}

	/*アニメーション処理*/
	{
		//アニメーションの設定
		_boss.SetNowAnimation(this->nextAnimation);
		//再生時間の設定
		_boss.SetAnimationPlayTime(this->animationPlayTime);
		//アニメーションの再生
		_boss.PlayAnimation();
	}

	/*攻撃判定の更新*/
	this->attack->Update();

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(this->maxInterval);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossSlashCombo2Action::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	this->parameter->desireValue = 0;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*状態がANGRYだったら欲求値を増加する*/
	else if (_boss.GetAngryState() >= this->checkState)
	{
		/*距離を求める*/
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//移動目標
		const VECTOR TO_TARGET	 = VSub(POSITION, MOVE_TARGET);	//目標から現在の移動目標へのベクトル
		const float  DISTANCE	 = VSize(TO_TARGET);					//距離

		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE <= this->actionDistance)
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//コンボ数が残っていなかったら早期リターン
			if (_boss.GetAttackComboCount() == 0) return;
			//前の攻撃がスラッシュコンボ２だったら
			if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = this->maxDesireValue;
			}
			//それ以外だったら
			else
			{
				this->parameter->desireValue = this->normalDisireValue;
			}
		}
	}
}