#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"

AttackSphereColliderData::AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data)
	: ColliderData(ColliderData::Kind::ATTACK_SPHERE, _priority, _tag)
	, radius(0.0f)
	, data(_data)
{
	//�����Ȃ�
}

void AttackSphereColliderData::OnHit(const CharacterData& _data)
{
	if (!_data.isInvinvible && !_data.isGuard)
	{
		this->data->isDoHitCheck = false;
		this->data->isHitAttack = true;
	}
}