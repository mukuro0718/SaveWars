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

	/*�v���C���[�ƃ{�X�Ƃ̓����蔻��*/
	//bool isHit = HitCheck_AABB_Sphere(enemy.GetCollider(), player.GetCollider());
	bool isHit = HitCheck_OBB_Sphere(enemy.GetCollider(), player.GetCollider());
	this->isHit = isHit;
}

/// <summary>
/// AABB��Sphere�̓����蔻��
/// </summary>
bool CollisionManager::HitCheck_AABB_Sphere(const Collider& _aabb, const Collider& _sphere)
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
bool CollisionManager::HitCheck_OBB_Sphere(const Collider& _obb, const Collider& _sphere)
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
