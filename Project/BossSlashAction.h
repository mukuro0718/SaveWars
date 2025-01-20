//=============================================
// @brief ボススラッシュ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashAction : public BossAction
{
public:
	 BossSlashAction();//コンストラクタ
	~BossSlashAction();//デストラクタ

	void Initialize		()					override;//初期化
	void Update			(Boss& _boss)		override;//更新
	void CalcParameter	(const Boss& _boss) override;//パラメーターの計算
private:
	/*定数*/
	const int	 ATTACK_TYPE;		//攻撃の種類
	const float  HIT_STOP_DELAY;	//ヒットストップディレイ
	const float	 HIT_STOP_TIME;		//ヒットストップ時間
	const int	 HIT_STOP_TYPE;		//ヒットストップの種類
	const float	 SLOW_FACTOR;		//スロー係数
	const short	 ROTATE_FIX_COUNT;	//回転補正用
	const VECTOR ROTATE_LERP_VALUE;	//回転補正量
	const short	 ROTATE_PLAY_TIME;	//回転補正用
	const short	 MOVE_PLAY_TIME;	//移動用
	const short	 STOP_PLAY_TIME;	//停止用
	const float	 STOP_DISTANCE;		//停止する距離
	const short  MAX_INTERVAL;		//最大インターバル
	const short	 CHECK_STATE;		//どの状態で確認するか
	const float	 ACTION_DISTANCE;	//アクションが可能になる距離
	const float  TARGET_OFFSET;		//目標補正

	/*メンバ変数*/
	bool	isClose;		//近づいたか
	float	speed;			//速度
};