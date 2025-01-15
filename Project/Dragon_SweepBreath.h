//=====================================================
// @brief なぎ払い＋ブレスアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_SweepBreath : public ActionNode
{
public:
	 Dragon_SweepBreath();//コンストラクタ
	~Dragon_SweepBreath();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		WALK   = 0,//歩き
		SWEEP  = 1,//なぎ払い
		BREATH = 2,//回転攻撃
	};

	/*メンバ変数*/
	vector<short>	animationType;		//アニメーションの種類
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			useAnimationType;	//使用するアニメーションのタイプ
	short			maxUseAnimation;	//使用するアニメーションの最大
	float			nearAttackRange;	//近接攻撃範囲
	float			sweepCancelPlayTime;//なぎ払い攻撃キャンセルフレーム
	float			totalPlayTime;		//総再生時間

};

