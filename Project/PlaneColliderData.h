//==========================================
// @brief ����(����͉~�`�ɂ���̂Ŕ��a������)��ColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class PlaneColliderData : public ColliderData
{
public:
	PlaneColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);
	VECTOR norm;//�@��
	float radius;
};
