//=========================================================
// �����蔻��f�[�^�N���X
//=========================================================
#pragma once
#include "GameObjectTag.h"

class Rigidbody;
class ColliderData abstract
{
public:
	/*�����蔻����*/
	enum class Kind
	{
		SPHERE,
		CAPSULE,
		PLANE,
		CUBE,
		CHARACTER_CAPSULE,
		ATTACK_SPHERE,
		ATTACK_CAPSULE,
	};
	/*�ʒu�␳�̗D��x����Ɏg��*/
	enum class Priority
	{
		LOW,	//��
		HIGH,	//��
		STATIC,//�����Ȃ�
	};

	/*�R���X�g���N�^*/
	ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag);
	/*�f�X�g���N�^*/
	virtual ~ColliderData();
	
	/*�����蔻���ʎ擾*/
	Kind		  GetKind	 ()const { return this->kind; }
	GameObjectTag GetTag	 ()const { return this->tag; }//�^�O���
	Priority	  GetPriority()const { return this->priority; }
	VECTOR GetNextPosition()const { return this->nextPositon; }
	void SetNextPosition(const VECTOR _position) { this->nextPositon = _position; }
	Rigidbody rigidbody;
protected:
	Kind kind;
	bool isHit;
	VECTOR nextPositon;
private:
	GameObjectTag tag;
	Priority priority;
};
