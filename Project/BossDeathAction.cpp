#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossDeathAction.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossDeathAction::BossDeathAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossDeathAction::~BossDeathAction()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossDeathAction::Initialize()
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
void BossDeathAction::Update(Boss& _boss)
{
	/*選択されていたら欲求値を０にする*/
	this->parameter->desireValue = 0;

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::DYING));

	/*アニメーションの再生*/
	_boss.PlayAnimation();

	/*もしアニメーションが終了していたら*/
	if (_boss.GetIsChangeAnimation())
	{
		_boss.OffIsAlive();
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_ENTRY);
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossDeathAction::CalcParameter(const Boss& _boss)
{
	/*追加する欲求値*/
	int addDesireValue = 0;

	/*HPが０以下だったら欲求値を最大にする*/
	if (_boss.GetHP() <= 0)
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}