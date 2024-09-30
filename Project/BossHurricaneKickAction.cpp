#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossHurricaneKickAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "BossAttackManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossHurricaneKickAction::BossHurricaneKickAction()
{
	this->isInitialize = false;
}

/// <summary>
/// デストラクタ
/// </summary>
BossHurricaneKickAction::~BossHurricaneKickAction()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossHurricaneKickAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossHurricaneKickAction::Update(Boss& _boss)
{
	/*選択されていたら*/
	if (this->isSelect)
	{
		/*シングルトンクラスのインスタンスの取得*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& effect = Singleton<EffectManager>::GetInstance();
		auto& attack = Singleton<BossAttackManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();

		/*使用する値の準備*/
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition(); //座標
		const VECTOR NEXT_MOVE_TARGET		= player.GetRigidbody().GetPosition();
			  VECTOR nowMoveTarget			= _boss.GetNowMoveTarget();//移動目標
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation(); //回転率
			  VECTOR positonToTargetVector	= VGet(0.0f, 0.0f, 0.0f); //プレイヤーから自分の座標までのベクトル
			  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);

		/*初期化されていなかったら*/
		if (!this->isInitialize)
		{
			//エフェクトを立てる
			//effect.OnIsBossSlashEffect();
			//攻撃フラグを立てる
			attack.OnIsStart(static_cast<int>(BossAttackManager::AttackType::HURRICANE_KICK));
			this->isInitialize = true;
			_boss.SetAnimationPlayTime(0.0f);
		}

		/*カウントの計測*/
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_ADD_FRAME_COUNT"]);
		
		/*移動目標*/
		if (isEndCount)
		{
			//現在の移動ベクトルを出す
			/*nowMoveTarget = _boss.Lerp(nowMoveTarget, NEXT_MOVE_TARGET, VGet(0.01f, 0.01f, 0.01f));*/
			//移動ベクトルの設定
			_boss.SetNowMoveTarget(NEXT_MOVE_TARGET);
			//プレイヤーから自分の座標までのベクトルを出す
			positonToTargetVector = VSub(POSITION, nowMoveTarget); 
		}

		/*回転処理*/
		if (isEndCount)
		{
			//アークタンジェントを使って角度を求める
			nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
			//回転率を代入
			_boss.SetRotation(nowRotation);
		}

		/*アニメーション再生時間の設定*/
		{
			float animationPlayTime = _boss.GetNowAnimationPlayTime();
			//カウントが終了していなければ
			animationPlayTime += static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_ADD_PLAY_TIME"]);
			if (animationPlayTime >= static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_MAX_PLAY_TIME"]))
			{
				animationPlayTime = static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_MAX_PLAY_TIME"]);
			}
			_boss.SetAnimationPlayTime(animationPlayTime);
		}

		/*移動スピードの設定*/
		float speed = 0.0f;
		{
			speed = json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_MOVE_SPEED"];
		}

		/*移動ベクトルを出す*/
		{
			//回転率をもとに移動ベクトルを出す
			direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
			//移動ベクトルを正規化
			direction = VNorm(direction);
			//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
			VECTOR aimVelocity = VScale(direction, speed);					 //算出された移動ベクトル
			VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //前の移動ベクトル
			VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//新しい移動ベクトル
			//移動ベクトルの設定
			_boss.SetVelocity(newVelocity);
		}

		/*終了判定*/
		if (_boss.GetIsChangeAnimation())
		{
			this->isInitialize = false;
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_INTERVAL"]);
		}
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossHurricaneKickAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*追加する欲求値*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*インターバルの設定*/
	//this->parameter->SetInterval(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_INTERVAL"]);

	///*インターバルが残っていたら欲求値を０にする*/
	//if (!this->parameter->CalcInterval()) addDesireValue = -this->parameter->MAX_PARAMETER;

	///*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
	//else if (_distance >= json.GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	//{
	//	addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE * 2;
	//}
	//else
	//{
	//	addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;
	//}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}