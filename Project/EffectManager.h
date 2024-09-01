#pragma once
#include "Singleton.h"

class Effect;
class EffectManager : public Singleton<EffectManager>
{
public:
	friend class Singleton<EffectManager>;

	/*エフェクトの描画用関数*/
	void OnIsBossPunchEffect();
	void OnIsBossSlashEffect();
	void OnIsBossThrowEffect();
	void Update();
	const void Draw()const;
private:
	/*静的定数*/
	static constexpr int PLAYER_EFFECT = 1;
	/*内部処理関数*/
	EffectManager();//コンストラクタ
	~EffectManager();//デストラクタ
	const VECTOR Convert(std::vector<float> _in)const;
	/*列挙体*/
	enum class BossEffect
	{
		PUNCH,
		SLASH,
		THROW
	};

	/*メンバ変数*/
	std::vector<Effect*> boss;
};

