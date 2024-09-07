//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class BitFlag;
class Physics;
class Collidable;
class Animation;
class Boss : public GoriLib::Collidable
{
public:
	Boss();//コンストラクタ
	~Boss();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);		//初期化
	void		Finalize(GoriLib::Physics* _physics);		//後処理
	void		Update(GoriLib::Physics* _physics);		//更新
	void		OnCollide(const Collidable& _colider)override;//衝突したとき
	const void	Draw()const;//描画

	/*getter*/
	const int	 GetDamage		()const;									  //ダメージの取得
	const int	 GetHP			()const;				  //HPの取得
	const VECTOR GetDirection	()const;
	const VECTOR GetRotation	()const;
	const VECTOR GetHeadPosition()const;
	const VECTOR GetPosition	()const;
	const int	 GetHitNumber	()const { return this->attackNumber; }
	const bool	 IsAttack		()const;
		  void	 OnIsHitAttack	()		{ this->isHitAttack = true; }
	const int	 GetModelHandle	()const { return this->modelHandle; }
	const VECTOR GetVelocity	()const { return this->GetVelocity(); }

	/*setter*/
	void SetHitNumber(const int _attackNumber)	{ this->attackNumber = _attackNumber; }
	void SetVelocity (const VECTOR _velocity)	{ this->rigidbody.SetVelocity(_velocity); }
	void SetRotation (const VECTOR _rotation)	{ this->rigidbody.SetRotation(_rotation); }
	void SetSpeed	 (const float _speed)		{ this->speed = _speed; }
	/*らーぷ関数*/
	float  Lerp(const float _start, const float _end, const float _percent);	//らーぷ関数
	VECTOR Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//らーぷ関数
private:
	/*ファンクション*/
	typedef std::function<void(void)> FlagsState;//フラグごとの実行したい関数（引数、返り値無し）

	/*静的定数*/
	static constexpr int COUNT_NUM = 6;
	//基本状態
	static constexpr unsigned int DYING	= (1 << 0);//デス
	static constexpr unsigned int IDLE	= (1 << 1);//待機
	static constexpr unsigned int ROAR	= (1 << 2);//咆哮
	static constexpr unsigned int WALK	= (1 << 3);//歩き
	static constexpr unsigned int REST	= (1 << 4);//休憩
	//攻撃
	static constexpr unsigned int SLASH			 = (1 << 5);//パンチ
	static constexpr unsigned int FLY_ATTACK	 = (1 << 6);//地面をたたく
	static constexpr unsigned int HURRICANE_KICK = (1 << 7);//回転蹴り
	static constexpr unsigned int JUMP_ATTACK	 = (1 << 8);//投石
	static constexpr unsigned int ROTATE_PUNCH	 = (1 << 9);//スラッシュ

	static constexpr unsigned int MASK_ATTACK = SLASH | ROTATE_PUNCH | JUMP_ATTACK |HURRICANE_KICK | FLY_ATTACK;
	static constexpr unsigned int MASK_ALL = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
	/*クラス*/
	class FlagsStateSet
	{
	public:
		FlagsState update;
	};
	/*列挙体*/
	//コライダーの種類
	enum class ColliderType
	{
		CHARACTER = 0,
		ATTACK = 1,
	};
	//フレームカウントの種類
	enum class FrameCountType
	{
		SLASH = 0,
		FLY_ATTACK = 1,
		HURRICANE_KICK = 2,
		JUMP_ATTACK = 3,
		ROTATE_PUNCH = 4,
		REST = 5,
	};
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
	};
	enum class AttackType
	{
		NONE			 = -1,
		SLASH			 = 0,//パンチ
		FLY_ATTACK		 = 1,//地面をたたく
		HURRICANE_KICK	 = 2,//回転蹴り
		JUMP_ATTACK		 = 3,//突き刺し攻撃
		ROTATE_PUNCH	 = 4,//回転パンチ
	};
	enum class AnimationType
	{
		DYING			= 0,//デス
		IDLE			= 1,//待機
		ROAR			= 2,//咆哮
		WALK			= 3,//歩き

		SLASH			= 4,//スラッシュ
		FLY_ATTACK		= 5,//飛び攻撃
		HURRICANE_KICK	= 6,//回転蹴り
		JUMP_ATTACK		= 7,//突き刺し攻撃
		ROTATE_PUNCH	= 8,//回転パンチ
	};

	/*内部処理関数*/
		  void Roar				();//咆哮
		  void UpdateRotation	();//回転率の更新
		  void UpdateMoveVector	();//移動ベクトルの更新
		  void UpdateSpeed		();//移動ベクトルの更新
		  void Move				();//移動
		  void Attack			();//攻撃
		  void Rest				();
		  void Death			();
		  void Reaction			();
		  void ChangeState		();
		  bool FrameCount		(const int _index, const int _maxFrame);
	const bool CanAttack		()const;//攻撃できるか
	const bool CanRotation		()const;//回転
	const bool CanMove			()const;//移動
	const bool CanRest			()const;//休憩できるか
	const bool CanRoar			()const;//咆哮できるか
		  void SetAttackComboCount();
		  void SetAttackCombo();
		  void SetPhase();
		  void AddItemFunction(const unsigned int _item, const FlagsState _update);//項目ごとの関数の追加
		  void OnEffectFlag(const int _attack);
		  void SetAttackFlag(const int _attack);
		  void SlowAnimationPlayTime(const FrameCountType _type, const int _targetCount, const float _maxTime);
		  void AddAnimationPlayTime(const FrameCountType _type, const int _targetCount, const float _maxTime);
		  void AttackSpeed(const int _type, const float _speed);

	/*メンバ変数*/
	std::map<unsigned int, int>	stateAnimationMap;		//項目ごとのアニメーション
	std::vector<int>			frameCount;				//フレームカウント
	std::vector<bool>			isCount;				//カウントをするか
	std::vector<int>			attackCombo;			//攻撃コンボ
	Animation*					animation;				//アニメーション
	BitFlag*					state;					//状態
	VECTOR						moveTarget;				//移動目標
	float						speed;					//速度
	float						animationPlayTime;		//アニメーション再生時間
	bool						isGround;				//地面と当たったか
	bool						isHitAttack;			//攻撃が当たったか
	bool						isDraw;					//描画するか
	int							attackComboCount;		//攻撃コンボ回数
	int							attackComboIndexOffset;	//攻撃コンボ回数を設定するためのインデックスオフセット
	int							attackNumber;			//攻撃の番号
	int							nowAnimation;			//現在のアニメーション
	int							attackType;				//攻撃の種類
	int							nowPhase;				//現在のフェーズ
	int							prevPhase;				//前のフェーズ
	int							modelHandle;			//モデルハンドル
};

