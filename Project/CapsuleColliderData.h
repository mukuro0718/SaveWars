//==========================================
// @brief �J�v�Z����ColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CapsuleColliderData : public ColliderData
{
public:
	/*�R���X�g���N�^*/
	CapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	/*�����o�ϐ�*/
	float	radius;		//���a
	VECTOR	topPositon;	//�J�v�Z������W
};
