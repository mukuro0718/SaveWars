#pragma once

class Physics;
class Collidable;
class PlayerAttack;
class PlayerAttackManager
{
public:
	PlayerAttackManager();//コンストラクタ
	~PlayerAttackManager();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);		//初期化
	void		Finalize(GoriLib::Physics* _physics);		//後処理
	void		Update(GoriLib::Physics* _physics);		//更新
	void		OnCollide(const GoriLib::Collidable& _colider);//衝突したとき
	const void	Draw()const;	//描画
	void		OnIsStart(const int _index);
private:
	enum class AttackType
	{
		NONE = -1,
		MAIN_1 = 0,//詠唱
		MAIN_2 = 1,//詠唱
		SPECIAL = 2,//コンボ
	};
	PlayerAttack* attack;
	int hitNumber;
};

