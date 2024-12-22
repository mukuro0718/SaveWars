#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "GameObjectTag.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackSphereColliderData.h"
#include "BossAttack.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAttack::BossAttack()
	: collider			(nullptr)
	, isStartHitCheck	(false)
	, isNotOnHit		(false)
	, frameCount		(0)
	, attackIndex		(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossAttack::~BossAttack()
{
	DeleteMemberInstance(this->collider);
}

/// <summary>
/// �㏈��
/// </summary>
void BossAttack::Finalize()
{
}

///// <summary>
///// �`��
///// </summary>
//const void BossAttack::Draw()const
//{
//#if _DEBUG
//	if (this->isStartHitCheck)
//	{
//		DrawSphere3D(this->rigidbody.GetPosition(), this->GetRadius(), 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
//	}
//	VECTOR position = this->rigidbody.GetPosition();
//	printfDx("MAIN_1_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
//#endif // _DEBUG
//}

/// <summary>
/// ���W�̎擾
/// </summary>
const VECTOR BossAttack::GetPosition()const
{
	return this->collider->rigidbody.GetPosition();
}


const bool BossAttack::GetIsHitAttack()
{
	if (this->collider->GetKind() == ColliderData::Kind::ATTACK_SPHERE)
	{
		auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
		return collider.data->isHitAttack;
	}
	else if (this->collider->GetKind() == ColliderData::Kind::ATTACK_CAPSULE)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		return collider.data->isHitAttack;
	}
	else
	{
		assert(0 && "������Ă��Ȃ��R���C�_�[�f�[�^�ł��B");
	}
}

void BossAttack::OffIsHitAttack()
{
	if (this->collider->GetKind() == ColliderData::Kind::ATTACK_SPHERE)
	{
		auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
		collider.data->isHitAttack = false;
	}
	else if (this->collider->GetKind() == ColliderData::Kind::ATTACK_CAPSULE)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		collider.data->isHitAttack = false;
	}
	else
	{
		assert(0 && "������Ă��Ȃ��R���C�_�[�f�[�^�ł��B");
	}
}

/// <summary>
/// std::vector<float>��VECTOR�ɕϊ�
/// </summary>
const VECTOR BossAttack::Convert(std::vector<float> _in)
{
	VECTOR out = VGet(0.0f, 0.0f, 0.0f);
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];
	return out;
}
