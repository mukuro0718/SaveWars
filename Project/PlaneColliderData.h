//========================================================
// @brief ����(����͉~�`�ɂ���̂Ŕ��a������)��ColliderData
//========================================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class PlaneColliderData : public ColliderData
{
public:
	/*�R���X�g���N�^*/
	PlaneColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	/*�����o�ϐ�*/
	VECTOR norm;	//�@��
	float  radius;	//���a
};
