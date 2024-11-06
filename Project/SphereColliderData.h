//==========================================
// @brief ����ColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class SphereColliderData : public ColliderData
{
public:
	/*�R���X�g���N�^*/
	SphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	/*�����o�ϐ�*/
	float radius;//���a
};
