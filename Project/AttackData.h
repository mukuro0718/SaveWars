#pragma once
class AttackData abstract
{
public:
	AttackData();
	virtual ~AttackData(){}
	bool isDoHitCheck;//当たり判定をするか
	int damage;//ダメージ
	int hitStopTime;
};

