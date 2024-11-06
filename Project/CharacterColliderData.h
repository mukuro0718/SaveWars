//==========================================
// @brief �_���[�W�p�J�v�Z��ColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class CharacterColliderData : public ColliderData
{
public:
	/*�R���X�g���N�^*/
	CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data);

	void OnHit(const AttackData& _data, const VECTOR _attackPosition);//�q�b�g����

	/*getter/setter*/
	const CharacterData& GetCharacterData ()const;			//�L�����N�^�[�f�[�^�̎擾
		  void			 SetPlayerReaction(const int _type);//�v���C���[���A�N�V�����̃Z�b�g

	/*�����o�ϐ�*/
	CharacterData*	data;		//�L�����N�^�[�f�[�^
	float			radius;		//���a
	VECTOR			topPositon;	//�J�v�Z������W
};
