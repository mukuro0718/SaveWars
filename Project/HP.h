//======================================================
// @brief HPクラス
// ボスとプレイヤーのHPの表示を行う
// （HPの直接管理は行わない）
//======================================================

#pragma once
class HP
{
public:
	HP			   ();		//コンストラクタ
	~HP			   ();		//デストラクタ
	void Update();
	const void Draw()const;	//描画
private:
	/*静的定数*/
	static const	 int BOSS_HP_COLOR;	 //ボスHPの色
	static const	 int PLAYER_HP_COLOR;//プレイヤーHPの色
	static const	 int PREV_HP_COLOR;	 //HPの差の色
	static constexpr int HP_BAR_HEIGHT		= 20;
	static constexpr int PLAYER_INDEX_BASE	= 300;//指数化のもと
	static constexpr int BOSS_INDEX_BASE	= 1000;//指数化のもと

	/*構造体*/
	struct RangeNum
	{
	public:
		void SetRange(const int _max, const int _min, const int _now);
		void Add(const int _add);
		const int GetMax()const { return this->max; }
		const int GetNow()const { return this->now; }
	private:
		int max, min, now;
	};
	struct Vec2d
	{
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		int x, y;
	};
	struct Box
	{
		int lx, ly, rx, ry;
	};

	/*メンバ変数*/
	RangeNum playerHP;	//プレイヤーHP
	RangeNum bossHP;	//ボスHP
	Vec2d playerHPPosition;
	Vec2d bossHPPosition;
};

