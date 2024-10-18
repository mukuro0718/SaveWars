#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseMath.h"
#include "GameObjectTag.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "BossData.h"
#include "PlayerData.h"
#include "AttackData.h"
#include "BossAttackData.h"
#include "PlayerAttackData.h"
#include "ColliderData.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
#include "PlaneColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackSphereColliderData.h"
#include "CharacterColliderData.h"
#include "Singleton.h"
#include "CollisionManager.h"

/// <summary>
/// �Փ˕��̓o�^
/// </summary>
void CollisionManager::Entry(ColliderData& _data)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(this->collidables.begin(), this->collidables.end(), &_data) != this->collidables.end());

	/*�o�^����Ă��Ȃ�������*/
	if (!found)
	{
		this->collidables.emplace_back(&_data);
	}
	/*�o�^����Ă�����G���[��f��*/
	else
	{
		assert(0 && "�w���collidable�͓o�^�ς݂ł�");
	}
}

/// <summary>
/// �Փ˕��̓o�^����
/// </summary>
void CollisionManager::Exit(ColliderData& _data)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(this->collidables.begin(), this->collidables.end(), &_data) != this->collidables.end());

	/*�o�^����Ă�����*/
	if (!found)
	{
		this->collidables.remove(&_data);
	}
	/*�o�^����Ă��Ȃ�������G���[��f��*/
	else
	{
		assert(0 && "�w���collidable�͓o�^����Ă��܂���");
	}

}

/// <summary>
/// �X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
/// </summary>
void CollisionManager::Update()
{
	//�ړ�
	for (auto& item : this->collidables)
	{
		//�|�W�V�����Ɉړ��͂𑫂�
		auto position = item->rigidbody.GetPosition();
		auto velocity = item->rigidbody.GetVelocity();
		//�d�͂𗘗p����ݒ�Ȃ�A�d�͂�ǉ�����
		if (item->rigidbody.UseGravity())
		{
			velocity = VAdd(velocity, VGet(0.0f, this->GRAVITY, 0));

			//�ő�d�͉����x���傫��������N�����v
			if (velocity.y < this->MAX_GRAVITY_ACCEL)
			{
				velocity = VGet(velocity.x, this->MAX_GRAVITY_ACCEL, velocity.z);
			}
		}

		auto nextPosition = VAdd(position, velocity);
		item->rigidbody.SetVelocity(velocity);

		//���Ƃ��Ƃ̏��A�\������f�o�b�O�\��
#if _DEBUG
		auto kind = item->GetKind();
		if (kind == ColliderData::Kind::ATTACK_SPHERE)
		{
			//AttackSphereColliderData& sphereData = dynamic_cast<AttackSphereColliderData&>(*item);
			//float radius = sphereData.radius;
			//DrawSphere3D(position, radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			//DrawSphere3D(nextPosition, radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
		else if (kind == ColliderData::Kind::ATTACK_CAPSULE)
		{
			//AttackCapsuleColliderData& capsuleData = dynamic_cast<AttackCapsuleColliderData&>(*item);
			//float radius = capsuleData.radius;
			//VECTOR topPositionOffset = capsuleData.topPositon;

			//DrawCapsule3D(position, VAdd(position, topPositionOffset), radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			//DrawCapsule3D(nextPosition, VAdd(nextPosition, topPositionOffset), radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
		else if (kind == ColliderData::Kind::CHARACTER_CAPSULE)
		{
			//CharacterColliderData& capsuleData = dynamic_cast<CharacterColliderData&>(*item);
			//float radius = capsuleData.radius;
			//VECTOR topPositionOffset = capsuleData.topPositon;

			//DrawCapsule3D(position, VAdd(position, topPositionOffset), radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			//DrawCapsule3D(nextPosition, VAdd(nextPosition, topPositionOffset), radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
#endif
		//�\��|�W�V�����ݒ�
		item->SetNextPosition(nextPosition);
	}
	//�����蔻��`�F�b�N
	std::vector<OnCollideInfo> onCollideInfo = CheckColide();

	//�ʒu�m��
	FixPosition();

}

/// <summary>
/// �����蔻��`�F�b�N
/// </summary>
std::vector<CollisionManager::OnCollideInfo> CollisionManager::CheckColide()
{
	std::vector<OnCollideInfo> onCollideInfo;
	//�Փ˒ʒm�A�|�W�V�����␳
	bool doCheck = true;
	int checkCount = 0;//�`�F�b�N��

	while (doCheck)
	{
		doCheck = false;
		checkCount++;

		//�Q�d���[�v�őS�I�u�W�F�N�g�����蔻��
		//(�d���̂ŃI�u�W�F�N�g���m�̂ݓ����蔻�肷��ȂǍH�v������)
		for (auto& objectA : this->collidables)
		{
			for (auto& objectB : this->collidables)
			{
				if (objectA != objectB)
				{
					//�Ԃ����Ă����
					if (IsCollide(*objectA, *objectB))
					{
						auto priorityA = objectA->GetPriority();
						auto priorityB = objectB->GetPriority();

						ColliderData* primary = objectA;
						ColliderData* secondary = objectB;

						//�v���C�I���e�B�̒Ⴂ�ق����ړ�
						if (priorityA < priorityB)
						{
							primary = objectB;
							secondary = objectA;
						}
						FixNextPosition(*primary, *secondary);

						break;
					}
				}
			}
			if (doCheck)
			{
				break;
			}
		}
		//�������[�v����
		if (checkCount > this->MAX_CHECK_COUNT && doCheck)
		{
#if _DEBUG
			printfDx("�����蔻��̌J��Ԃ��`�F�b�N���K�萔�𒴂���\n");
#endif
			break;
		}
	}
	return onCollideInfo;
}

/// <summary>
/// �w�胉�C�����I�u�W�F�N�g�ƂԂ����Ă��邩�ǂ������肵�A�Ԃ����Ă���I�u�W�F�N�g��Ԃ�
/// </summary>
//std::list<Collidable*> CollisionManager::IsCollideLine(const VECTOR& _start, const VECTOR& _end)const
//{
//	std::list<Collidable*> ret;
//
//	for (auto& object : collidables)
//	{
//		//collidable�̎�ނɂ���āA�����蔻��𕪂���
//		auto kind = object->GetKind();
//		//���C���ƕ���
//		if (kind == ColliderData::Kind::PLANE)
//		{
//			auto planeColliderData = dynamic_cast<ColliderDataPlane*>(object);
//			//���C���Ɠ_�̍ŒZ���������߂�
//			float minLength = Segment_Point_MinLength(_start, _end, object->rigidbody.GetPosition());
//			//float�̌덷���l�����Ĕ���
//			if (minLength < 0.001f)
//			{
//				ret.emplace_back(object);
//			}
//		}
//	}
//	return ret;
//}

/// <summary>
/// �������Ă��邩�ǂ�����������
/// </summary>
bool CollisionManager::IsCollide(ColliderData& _objectA, ColliderData& _objectB)
{
	bool isHit = false;

	/*collidable�̎�ނɂ���ē����蔻��𕪂���*/
	auto aKind = _objectA.GetKind();
	auto bKind = _objectB.GetKind();

	/*���Ƌ�*/
	if (aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::SPHERE)
	{
		//auto aTob = VSub(_objectB.GetNextPosition(), _objectA.GetNextPosition());
		//auto aTobLength = VSize(aTob);

		///*�݂��̋������A���ꂼ��̔��a�̍��v������������Γ�����*/
		//auto objectAColliderData = dynamic_cast<ColliderDataSphere*>(_objectA);
		//auto objectBColliderData = dynamic_cast<ColliderDataSphere*>(_objectB);
		//isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}
	/*�J�v�Z���ƃJ�v�Z��*/
	else if (aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto aTob = VSub(_objectB.GetNextPosition(), _objectA.GetNextPosition());
		auto aTobLength = VSize(aTob);

		/*�݂��̋������A���ꂼ��̔��a�̍��v������������Γ�����*/
		auto& objectAColliderData = dynamic_cast<CharacterColliderData&>(_objectA);
		auto& objectBColliderData = dynamic_cast<CharacterColliderData&>(_objectB);
		isHit = (aTobLength < objectAColliderData.radius + objectBColliderData.radius);
	}

	/*���ƃJ�v�Z��*/
	else if ((aKind == ColliderData::Kind::ATTACK_SPHERE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_SPHERE))
	{
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			ColliderData* attackDataBase = &_objectA;
			VECTOR attackSphereCenter = _objectA.GetNextPosition();
			ColliderData* characterDataBase = &_objectB;
			VECTOR characterCapsuleUnder = _objectB.GetNextPosition();
			if (bKind == ColliderData::Kind::ATTACK_SPHERE)
			{
				attackDataBase = &_objectB;
				attackSphereCenter = _objectB.GetNextPosition();
				characterDataBase = &_objectA;
				characterCapsuleUnder = _objectA.GetNextPosition();
			}
			auto& sphereColliderData = dynamic_cast<AttackSphereColliderData&>(*attackDataBase);
			auto& capsuleColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			if (sphereColliderData.data->isDoHitCheck)
			{
				VECTOR characterCapsuleTop = VAdd(characterCapsuleUnder,capsuleColliderData.topPositon);
				isHit = HitCheck_Sphere_Capsule(attackSphereCenter, sphereColliderData.radius, characterCapsuleUnder, characterCapsuleTop, capsuleColliderData.radius);
			}
		}
	}

	/*�J�v�Z���ƃJ�v�Z��*/
	else if ((aKind == ColliderData::Kind::ATTACK_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_CAPSULE))
	{
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			ColliderData* attackDataBase = &_objectA;
			VECTOR attackCapsuleUnder = _objectA.GetNextPosition();
			ColliderData* characterDataBase = &_objectB;
			VECTOR characterCapsuleUnder = _objectB.GetNextPosition();
			if (bKind == ColliderData::Kind::ATTACK_CAPSULE)
			{
				attackDataBase = &_objectB;
				attackCapsuleUnder = _objectB.GetNextPosition();
				characterDataBase = &_objectA;
				characterCapsuleUnder = _objectA.GetNextPosition();
			}
			auto& attackColliderData = dynamic_cast<AttackCapsuleColliderData&>(*attackDataBase);
			auto& characterColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			if (attackColliderData.data->isDoHitCheck)
			{
				VECTOR attackCapsuleTop = attackColliderData.topPositon;
				VECTOR characterCapsuleTop = VAdd(characterCapsuleUnder, characterColliderData.topPositon);
				isHit = HitCheck_Capsule_Capsule(attackCapsuleUnder, attackCapsuleTop, attackColliderData.radius, characterCapsuleUnder, characterCapsuleTop, characterColliderData.radius);
			}
		}
	}

	/*�J�v�Z���ƕ��ʂ̓����蔻��*/
	else if ((aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::CHARACTER_CAPSULE))
	{
		ColliderData* planeDataBase = &_objectA;
		VECTOR planeCenter = _objectA.rigidbody.GetPosition();
		VECTOR capsuleUnder = _objectB.GetNextPosition();
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = &_objectB;
			planeCenter = _objectB.GetNextPosition();
			capsuleUnder = _objectA.GetNextPosition();
		}
		auto& planeColliderData = dynamic_cast<PlaneColliderData&>(*planeDataBase);

		if (bKind == ColliderData::Kind::CHARACTER_CAPSULE)
		{
			capsuleUnder = _objectB.GetNextPosition();
		}
		/*���͒n�ʂ��~�`�̕��ʂ������Ă���̂ŁA�����蔻���Y���W(�O�ȉ����ǂ���)�ƕ��ʂ̒��S���W�����ʂ̔��a���Ȃ����𔻒肷��*/
		float distance = VSize(VSub(capsuleUnder, planeCenter));
		if ((capsuleUnder.y < 0.0f) || (distance > planeColliderData.radius))
		{
			isHit = true;
		}
	}
	/*���ƕ��ʂ̓����蔻��*/
	else if ((aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::SPHERE))
	{
		ColliderData* planeDataBase = &_objectA;
		VECTOR planeCenter = _objectA.GetNextPosition();
		VECTOR sphereCenter = _objectB.GetNextPosition();
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = &_objectB;
			planeCenter = _objectB.GetNextPosition();
			sphereCenter = _objectA.GetNextPosition();
		}
		auto& planeColliderData = dynamic_cast<PlaneColliderData&>(*planeDataBase);

		if (bKind == ColliderData::Kind::SPHERE)
		{
			sphereCenter = _objectB.GetNextPosition();
		}
		/*���͒n�ʂ��~�`�̕��ʂ������Ă���̂ŁA�����蔻���Y���W(�O�ȉ����ǂ���)�ƕ��ʂ̒��S���W�����ʂ̔��a���Ȃ����𔻒肷��*/
		float distance = VSize(VSub(sphereCenter, planeCenter));
		if ((sphereCenter.y < 0.0f) || (distance > planeColliderData.radius))
		{
			isHit = true;
		}
	}

	return isHit;
}


void CollisionManager::FixNextPosition(ColliderData& _primary, ColliderData& _secondary)
{
	/*�����蔻��̎�ʂ��Ƃɕ␳���@��ς���*/
	auto primaryKind = _primary.GetKind();
	auto secondaryKind = _secondary.GetKind();
	//�����m�̈ʒu�␳
	if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		//VECTOR primaryToSecondary = VSub(secondary.GetNextPosition(), primary.GetNextPosition());
		//VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		//auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary);
		//auto secondaryColliderData = dynamic_cast<ColliderDataSphere*> (secondary);
		////���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
		//float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		//VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		//VECTOR fixedPosition = VAdd(primary.GetNextPosition(), primaryToNewSecondaryPosition);
		//secondary.GetNextPosition() = fixedPosition;
	}
	//�J�v�Z�����m�̈ʒu�␳
	else if (primaryKind == ColliderData::Kind::CHARACTER_CAPSULE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		VECTOR secondaryToPrimary = VSub(_primary.GetNextPosition(), _secondary.GetNextPosition());
		VECTOR secondaryToPrimaryNorm = VNorm(secondaryToPrimary);
		float  secondaryToPrimarySize = VSize(secondaryToPrimary);

		auto& primaryColliderData = dynamic_cast<CharacterColliderData&> (_primary);
		auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);

		//���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
		float radiusSum = primaryColliderData.radius + secondaryColliderData.radius;
		float awayDist = radiusSum - secondaryToPrimarySize + 0.005f;
		VECTOR fixedSize = VScale(secondaryToPrimaryNorm, awayDist);
		VECTOR fixedPosition = VAdd(_primary.GetNextPosition(), fixedSize);
		_primary.SetNextPosition(fixedPosition);
	}
	//���ʂƃJ�v�Z��(���ʂ�STATIC�Ȃ̂ŁA�K��primary��PLANE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto& primaryColliderData = dynamic_cast<PlaneColliderData&> (_primary);

		VECTOR fixedPosition = _secondary.GetNextPosition();
		VECTOR secondaryToPrimary = VSub(_secondary.GetNextPosition(), _primary.rigidbody.GetPosition());
		float distance = VSize(secondaryToPrimary);
		float fixValue = 0.0f;

		if (distance > primaryColliderData.radius)
		{
			fixValue = primaryColliderData.radius - distance;
			fixedPosition = VAdd(fixedPosition, VScale(VNorm(secondaryToPrimary), fixValue));
		}
		fixedPosition.y = 0.0f;
		_secondary.SetNextPosition(fixedPosition);
	}
	//���ʂƃX�t�B�A(���ʂ�STATIC�Ȃ̂ŁA�K��primary��PLANE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		auto& primaryColliderData = dynamic_cast<PlaneColliderData&> (_primary);

		VECTOR fixedPosition = _secondary.GetNextPosition();
		if (fixedPosition.y < 0.0f)
		{
			fixedPosition.y = 0.0f;
		}
		_secondary.SetNextPosition(fixedPosition);
	}
	//���ƃJ�v�Z��(����STATIC�Ȃ̂ŁA�K��primary��SPHERE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::ATTACK_SPHERE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		
		auto& primaryColliderData = dynamic_cast<AttackSphereColliderData&> (_primary);
		auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		if (primaryColliderData.data->isDoHitCheck)
		{
			primaryColliderData.OnHit(*secondaryColliderData.data);
			secondaryColliderData.OnHit(*primaryColliderData.data, primaryColliderData.GetNextPosition());
			if (_primary.GetTag() == GameObjectTag::BOSS_ATTACK && _secondary.GetTag() == GameObjectTag::PLAYER)
			{
				auto& primaryAttackData = dynamic_cast<BossAttackData&> (*primaryColliderData.data);
				secondaryColliderData.SetPlayerReaction(primaryAttackData.playerReaction);
			}
		}
	}

	//���ƃJ�v�Z��(����STATIC�Ȃ̂ŁA�K��primary��SPHERE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::ATTACK_CAPSULE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto& primaryColliderData = dynamic_cast<AttackCapsuleColliderData&> (_primary);
		auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		if (primaryColliderData.data->isDoHitCheck)
		{
			primaryColliderData.OnHit(*secondaryColliderData.data);
			secondaryColliderData.OnHit(*primaryColliderData.data,primaryColliderData.GetNextPosition());
			if (_primary.GetTag() == GameObjectTag::BOSS_ATTACK && _secondary.GetTag() == GameObjectTag::PLAYER)
			{
				auto& primaryAttackData = dynamic_cast<BossAttackData&> (*primaryColliderData.data);
				secondaryColliderData.SetPlayerReaction(primaryAttackData.playerReaction);
			}
		}
	}
	else
	{
		assert(0 && "������Ă��Ȃ������蔻��̈ʒu�␳�ł�");
	}
}

/// <summary>
/// �ʒu�m��
/// </summary>
void CollisionManager::FixPosition()
{
	for (auto& item : collidables)
	{
		//position���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		VECTOR toFixedPosition = VSub(item->GetNextPosition(), item->rigidbody.GetPosition());
		item->rigidbody.SetVelocity(toFixedPosition);

		//�ʒu�m��
		item->rigidbody.SetPosition(item->GetNextPosition());
	}
}

