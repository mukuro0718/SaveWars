//================================================
// @brief �{�X�A�N�V�����̊��N���X
//================================================
#pragma once

class Character;
class Rigidbody;
class BossAction abstract
{
public:
	BossAction(){}
	virtual ~BossAction(){}

	virtual void Initialize	() abstract;
	virtual void Finalize	() abstract;
	virtual Rigidbody& Update		(Character& _boss) abstract;

protected:
};

