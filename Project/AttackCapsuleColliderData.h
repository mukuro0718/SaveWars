//==========================================
// @brief �U���p�̃J�v�Z��ColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class AttackCapsuleColliderData : public ColliderData
{
public:
	/*�R���X�g���N�^*/
	AttackCapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);

	void OnHit(const CharacterData& _data);//�����������̏���

	/*�����o�ϐ�*/
	AttackData* data;		//�U���f�[�^
	VECTOR		topPositon;	//�J�v�Z������W
	float		radius;		//���a
};
