//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class PlayerController;
class Character;
class HitStop;
class PlayerAction;
class Player : public Character
{
public:
	Player();//コンストラクタ
	~Player();//デストラクタ

	void		Initialize		 () override;		//初期化
	void		Finalize		 () override;		//後処理
	void		Update			 () override;		//更新
	const void	DrawCharacterInfo()const override;	//描画
	void PlayAnimation(const int _nextAnimation, const float _playTime);
	void DeathProcess();
	const bool CanAction		(const float _staminaConsumed)const;//アクションができるか
		  void CalcStamina		(const float _staminaConsumed);			//スタミナの回復処理

	/*getter*/
	const bool		GetIsAttack			()const override { return false; }
	const int		GetStamina			()const;								//スタミナの取得
	const int		GetHealCount		()const { return this->healCount; }	//回復オーブの数(今は回復回数になっている)
	CharacterData&	GetPlayerData		();
	Rigidbody&		GetPlayerRigidbody	();
		  void		SetHealCount		(const int _count) { this->healCount = _count; }								//回復回数の設定
		  void		SetHitStop			(const float _time, const int _type, const float _delay, const float _factor);	//ヒットストップの設定

	//アニメーションの種類
	enum class AnimationType
	{
		IDLE = 0,
		AVOID = 1,
		DEATH = 2,
		BLOCK = 3,
		REACTION = 4,
		BLOCK_REACTION = 5,
		WALK_FRONT = 6,
		WALK_BACK = 7,
		WALK_LEFT = 8,
		WALK_RIGHT = 9,
		RUN_FRONT = 10,
		RUN_BACK = 11,
		RUN_LEFT = 12,
		RUN_RIGHT = 13,
		RUN_180_TURN = 14,
		COMBO_1 = 15,
		HEAL = 16,
		COMBO_2 = 17,
		COMBO_3 = 18,
		SKILL = 19,
		DOWN_REACTION = 20,
		DOWN_UP = 21,
	};

private:
	/*静的定数*/
	static constexpr float	SHADOW_HEIGHT	= 10.0f;//影を投影する高さ
	static constexpr float	SHADOW_SIZE		= 6.0f;	//かげのサイズ
	
	/*メンバ変数*/
	PlayerController*		controller;
	vector<PlayerAction*>	action;
	HitStop*				hitStop;			//ヒットストップ
	int						healCount;		//回復オーブの数
};

