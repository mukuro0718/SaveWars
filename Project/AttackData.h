#pragma once
class AttackData abstract
{
public:
	AttackData();
	virtual ~AttackData(){}
	bool isDoHitCheck;//�����蔻������邩
	int damage;//�_���[�W
	int hitStopTime;
};

