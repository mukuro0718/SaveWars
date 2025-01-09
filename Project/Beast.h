//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class BeastBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Beast : public Enemy
{
public:
	 Beast();//コンストラクタ
	~Beast();//デストラクタ

	void		Initialize			() override;		//初期化
	void		Finalize			() override;		//後処理
	void		Update				() override;		//更新
	const void	DrawCharacterInfo	() const override;	//描画
	void		PlayAnimation		();					//アニメーションの再生

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		DYING						= 0, //デス(97)
		IDLE						= 1, //待機(111)
		ROAR						= 2, //咆哮(通常→怒り状態の変化110)
		WALK						= 3, //歩き(121)
		RUN							= 4, //ダッシュ(117)
		TURN_LEFT					= 5, //左を向く(119)
		TURN_RIGHT					= 6, //右を向く(120)
		DOWN						= 7, //ダウン(4)
		LONG_FRIGHT_START			= 8, //長い怯み(開始104)
		LONG_FRIGHT_LOOP			= 9, //長い怯み(途中102)
		LONG_FRIGHT_END				= 10,//長い怯み(終了100)
		SHORT_FRIGHT_START			= 11,//短い怯み(開始107)
		SHORT_FRIGHT_LOOP			= 12,//短い怯み(開始106)
		SHORT_FRIGHT_END			= 13,//短い怯み(終了105)
		REST_START					= 14,//休憩(開始89)
		REST_LOOP					= 15,//休憩(途中90)
		REST_END					= 16,//休憩(終了91)
		STEP_BACK					= 17,//飛び下がる(8)
		RAISE_LEVEL					= 18,//チャージレベルの上昇(14)
		BACKING_BREATH				= 19,//下がりながらブレス(10)
		RUSH_START					= 20,//突進(開始75)
		RUSH_LOOP					= 21,//突進(途中76)
		RUSH_ROTATE_END				= 22,//突進(回転して止まる77)
		RUSH_NORMAL_END				= 23,//突進(普通に止まる78)
		CHARGE_RIGHT_ATTACK			= 24,//右前足溜め攻撃(52)
		CHARGE_BOTH_ATTACK_START	= 25,//両前足溜め攻撃(開始50)
		CHARGE_BOTH_ATTACK_END		= 26,//両前足溜め攻撃(終了51)
		BREATH_START				= 27,//ブレス(開始45)
		BREATH_LOOP					= 28,//ブレス(途中46)
		BREATH_END					= 29,//ブレス(終了47)
		SUPER_NOVA_START			= 30,//スーパーノヴァ(開始27)
		SUPER_NOVA_LOOP				= 31,//スーパーノヴァ(途中28)
		SUPER_NOVA_END				= 32,//スーパーノヴァ(終了29)
		WEAK_BREATH					= 33,//弱ブレス(80)
		SMALL_EXPLOSION				= 34,//小爆発(9)
		RIGHT_FOOT_ATTACK			= 35,//右足攻撃(40)
		EXPLOSION					= 36,//爆発攻撃（スーパーノヴァよりも小さい）(65)
		COMBO_ATTACK				= 37,//右足で攻撃した後回転攻撃(22)
	};
	enum class Parts
	{
		HEAD,
		BODY,
		LEFT_HAND,
		RIGHT_HAND,
		LEFT_FOOT,
		RIGHT_FOOT,
	};

	/*getter/setter*/
	const bool		 GetIsAttack			()const override;													//コウゲキしたか
		  void		 SetRotation			(const VECTOR _rotation);											//回転率の設定
		  void		 SetVelocity			(const VECTOR _velocity);											//移動ベクトルの設定
		  void		 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//現在の移動目標を設定
		  void		 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//速さの設定
	const float		 GetAnimationPlayTime	()const;															//アニメーション再生時間の取得
	const float		 GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//現在のアニメーション再生時間の取得
		  void		 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//アニメーション再生時間の設定
		  void		 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//現在のアニメーションの設定
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 8.0f; //影のサイズ
	
	/*メンバ変数*/
	vector<CharacterColliderData*>  partsCollider;				//部位ごとのコライダー
	short							maxPartsColliderNum;		//コライダーの数
	VECTOR							moveTarget;					//移動目標
	float							animationPlayTime;			//アニメーション再生時間
	int								nowAnimation;				//現在のアニメーション
	vector<vector<short>>			frameIndexUsePartsColider;	//部位ごとのコライダーの指定に使用するフレーム番号
	vector<VECTOR>					pos1;						//部位ごとのコライダーの座標を入れている
	vector<VECTOR>					pos2;						//HACK:なぜかいったん代入しないとうまく入らない、なぜ
	vector<float>					prevPartsHp;				//前の部位ごとのHP
	float							maxHp;						//部位の最大HP
};

