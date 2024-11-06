//==========================================
// @brief �U���p�̃X�t�B�AColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class AttackData;
class CharacterData;
class AttackSphereColliderData : public ColliderData
{
public:
	/*�R���X�g���N�^*/
	AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);

	void OnHit(const CharacterData& _data);//�����������̏���

	/*�����o�ϐ�*/
	AttackData* data;	//�U���f�[�^
	float		radius;	//���a
};
