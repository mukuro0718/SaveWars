#include <DxLib.h>
#include "CollisionManager.h"
#include "UseMath.h"
#include "Vector4.h"
#include "Collider.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

CollisionManager::CollisionManager()
	: isHit(false)
	, debugMax{ 0.0f,0.0f,0.0f }
	, debugMin{ 0.0f,0.0f,0.0f }
{

}

/// <summary>
/// �����蔻��
/// </summary>
void CollisionManager::HitCheck()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();

	this->fixVector = { 0.0f,0.0f,0.0f };

	/*�v���C���[�ƃ{�X�Ƃ̓����蔻��*/
	if (HitCheckSphereSphere(player.GetCharacterCollider(), enemy.GetCharacterCollider()))
	{
		player.FixMoveVector(this->fixVector);
	}

	/*�v���C���[�̍U���X�t�B�A�ƃ{�X�̃J�v�Z���Ƃ̓����蔻��*/
	if (player.IsAttack())
	{
		if (HitCheckSphereCapsule(player.GetAttackCollider(), enemy.GetCharacterCollider()))
		{
			int attackNumber = player.GetAttackNumber();
			int hitNumber = enemy.GetHitNumber();
			if (attackNumber != hitNumber)
			{
				enemy.CalcDamage(player.GetDamage());
				enemy.SetHitNumber(attackNumber);
			}
		}
	}

	/*�{�X�̍U���ƃv���C���[�̃J�v�Z���Ƃ̓����蔻��*/
	if (enemy.IsAttack())
	{
		if (HitCheckSphereCapsule(enemy.GetAttackCollider(), player.GetCharacterCollider()))
		{
			int attackNumber = enemy.GetAttackNumber();
			int hitNumber = player.GetHitNumber();
			if (attackNumber != hitNumber)
			{
				player.CalcDamage(enemy.GetDamage());
				player.SetHitNumber(attackNumber);
			}
		}
	}
}

/// <summary>
/// �X�t�B�A�ƃX�t�B�A�̓����蔻��
/// </summary>
bool CollisionManager::HitCheckSphereSphere(const Collider& _sphere1, const Collider& _sphere2)
{
	/*�X�t�B�A�Ԃ̋��������߂�*/
	float betweenSphereDistance = VSize(VSub(_sphere1.position, _sphere2.position));

	/*�X�t�B�A�̔��a�̍��v�����߂�*/
	float radiusSub = _sphere1.radius + _sphere2.radius;

	/*�����X�t�B�A�Ԃ̋��������a�̍��v����������������*/
	if (betweenSphereDistance <= radiusSub)
	{
		float fixSize = radiusSub - betweenSphereDistance;
		VECTOR fixVector = VNorm(VSub(_sphere1.position, _sphere2.position));
		fixVector = VScale(fixVector, fixSize);
		this->fixVector = fixVector;
		return true;
	}

	return false;
}

/// <summary>
/// �J�v�Z���ƃX�t�B�A�̓����蔻��
/// </summary>
bool CollisionManager::HitCheckSphereCapsule(const Collider& _sphere, const Collider& _capsule)
{
	return HitCheck_Sphere_Capsule(_capsule.position, _capsule.radius, _capsule.position, _capsule.heightPosition, _capsule.radius);
}

/// <summary>
/// AABB��Sphere�̓����蔻��
/// </summary>
bool CollisionManager::HitCheckAABBSphere(const Collider& _aabb, const Collider& _sphere)
{
	/*AABB��sphere�̍ŋߐړ_�����߂�*/
	float length = GetLengthAABBToPoint(_aabb, _sphere);

	/*�ŋߐړ_���O�Ƃ������Ƃ́Asphere�̒��S���W��AABB�̒��ɂ���Ƃ������ƂȂ̂ő������^�[��*/
	if (length == 0.0f)return true;

	/*�ŋߐړ_��sphere�̔��a�������������瓖�����Ă���*/
	if (length < _sphere.radius) return true;

	return false;
}

/// <summary>
/// OBB�{�b�N�X�����蔻��
/// </summary>
bool CollisionManager::HitCheckOBBSphere(const Collider& _obb, const Collider& _sphere)
{
	float OBBToSphere = VSize(VSub(_obb.position, _sphere.position));

	/*OBB��sphere�̍ŋߐړ_�����߂�*/
	float length = GetLengthOBBToPoint(_obb, _sphere);

	/*�ŋߐړ_���O�Ƃ������Ƃ́Asphere�̒��S���W��OBB�̒��ɂ���Ƃ������ƂȂ̂ő������^�[��*/
	if (length == 0.0f)return true;

	/*�ŋߐړ_��sphere�̔��a�������������瓖�����Ă���*/
	if (length < _sphere.radius) return true;

	return false;
}

/// <summary>
/// AABB�I�u�W�F�N�g��1�_�̍ŋߐړ_�����߂�
/// </summary>
float CollisionManager::GetLengthAABBToPoint(const Collider& _aabb, const Collider& _point)
{
	float SqLen = 0;   // �����ׂ̂���̒l���i�[
	int i;
	for (i = 0; i < 3; i++)
	{
		// �e���œ_���ŏ��l�ȉ��������͍ő�l�ȏ�Ȃ�΁A�����l��
		if (_point.GetAxisValue(i) < _aabb.GetMin(i))  // i=0��X�A1��Y�A2��Z�̈Ӗ��ł�
			SqLen += (_point.GetAxisValue(i) - _aabb.GetMin(i)) * (_point.GetAxisValue(i) - _aabb.GetMin(i));
		if (_point.GetAxisValue(i) > _aabb.GetMax(i))
			SqLen += (_point.GetAxisValue(i) - _aabb.GetMax(i)) * (_point.GetAxisValue(i) - _aabb.GetMax(i));
	}
	return static_cast<float>(sqrt(SqLen));
}

float CollisionManager::GetLengthOBBToPoint(const Collider& _obb, const Collider& _point)
{
	/*�ŏI�I�ɒ��������߂�x�N�g��*/
	VECTOR vector = { 0.0f, 0.0f, 0.0f };

	/*�e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o*/
	for (int i = 0; i < 3; i++)
	{
		float length = _obb.GetLength(i);
		if (length <= 0) continue;  // L=0�͌v�Z�ł��Ȃ�

		VECTOR pointToObb = VSub(_obb.position, _point.position);
		VECTOR direction = _obb.GetDirection(i);
		float dot = VDot(pointToObb, direction);
		float size = dot / length;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		size = static_cast<float>(fabs(size));
		if (size > 1.0f)
		{
			//�͂ݏo���������̃x�N�g���Z�o
			float scale = (1.0f - size) * length;
			vector = VAdd(vector, VScale(direction, scale));
		}
	}

	/*�������o��*/
	return VSize(vector);
}

const void CollisionManager::DebugDrawHit()const
{
	DrawCube3D(debugMax, debugMin, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
	if (this->isHit)
	{
		//printfDx("HIT\n");
	}
	else
	{
		//printfDx("NOT_HIT\n");
	}
}
