#pragma once
class CharacterData abstract
{
public:
	CharacterData();
	virtual ~CharacterData(){}

	int hp;//HP
	bool isHit;//�U���Ƀq�b�g������
	VECTOR hitPosition;
	bool isCutDamage;
	int playerReaction;
	int hitStopTime;
	int hitStopType;
	int hitStopDelay;
	float slowFactor;
};

