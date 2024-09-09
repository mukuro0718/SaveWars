#include <cassert>
#include <DxLib.h>
#include "GoriLib.h"
#include "UseMath.h"

using namespace GoriLib;

/// <summary>
/// �Փ˕��̓o�^
/// </summary>
void Physics::Entry(Collidable* _collidable)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*�o�^����Ă��Ȃ�������*/
	if (!found)
	{
		collidables.emplace_back(_collidable);
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
void Physics::Exit(Collidable* _collidable)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*�o�^����Ă�����*/
	if (!found)
	{
		collidables.remove(_collidable);
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
void Physics::Update()
{
	//�ړ�
	for (auto& item : collidables)
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
				velocity = VGet(velocity.x, this->MAX_GRAVITY_ACCEL, velocity.y);
			}
		}

		auto nextPosition = VAdd(position, velocity);
		item->rigidbody.SetVelocity(velocity);

		//���Ƃ��Ƃ̏��A�\������f�o�b�O�\��
#if _DEBUG
		auto kind = item->colliderData->GetKind();
		if (kind == ColliderData::Kind::SPHERE)
		{
			ColliderDataSphere* sphereData;
			sphereData = dynamic_cast<ColliderDataSphere*>(item->colliderData);
			float radius = sphereData->radius;
			DrawSphere3D(position, radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			DrawSphere3D(nextPosition, radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
		else if (kind == ColliderData::Kind::CAPSULE)
		{
			ColliderDataCapsule* capsuleData;
			capsuleData = dynamic_cast<ColliderDataCapsule*>(item->colliderData);
			float radius = capsuleData->radius;
			VECTOR topPositionOffset = VGet(0.0f, capsuleData->height, 0.0f);

			DrawCapsule3D(position, VAdd(position, topPositionOffset), radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			DrawCapsule3D(nextPosition, VAdd(nextPosition, topPositionOffset), radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
#endif
		//�\��|�W�V�����ݒ�
		item->nextPosition = nextPosition;
	}
	//�����蔻��`�F�b�N
	std::vector<OnCollideInfo> onCollideInfo = CheckColide();

	//�ʒu�m��
	FixPosition();

	//������ʒm
	for (auto& item : onCollideInfo)
	{
		item.owner->OnCollide(*item.colider);
	}
}

/// <summary>
/// �����蔻��`�F�b�N
/// </summary>
std::vector<Physics::OnCollideInfo> Physics::CheckColide()const
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
					if (IsCollide(objectA, objectB))
					{
						auto priorityA = objectA->GetPriority();
						auto priorityB = objectB->GetPriority();

						Collidable* primary = objectA;
						Collidable* secondary = objectB;

						//�ǂ�����g���K�[�o�Ȃ���Ύ��ڕW�ʒu�C��
						bool isTriggerAorB = objectA->colliderData->IsTrigger() || objectB->colliderData->IsTrigger();
						if (!isTriggerAorB)
						{
							//�v���C�I���e�B�̒Ⴂ�ق����ړ�
							if (priorityA < priorityB)
							{
								primary = objectB;
								secondary = objectA;
							}
							FixNextPosition(primary, secondary);
						}

						//�Փˏ��̍X�V
						//�iprimary��secondary��������Ă΂��\���͂���̂ŁA�r���x�������j
						bool hasPrimaryInfo = false;
						bool hasSecondaryInfo = false;
						for (const auto& item : onCollideInfo)
						{
							//���łɒʒm���X�g�Ɋ܂܂�Ă�����Ă΂Ȃ�
							if (item.owner == primary)
							{
								hasPrimaryInfo = true;
							}
							if (item.owner == secondary)
							{
								hasPrimaryInfo = true;
							}
						}
						if (!hasPrimaryInfo)
						{
							onCollideInfo.push_back({ primary,secondary });
						}
						if (!hasSecondaryInfo)
						{
							onCollideInfo.push_back({ secondary,primary });
						}

						//��x�ł��q�b�g�{�␳������Փ˔���ƕ␳��蒼��
						//�Е����g���K�[�Ȃ�q�b�g��蒼���Ȃ�
						if (!isTriggerAorB)
						{
							doCheck = true;
						}
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
		if (checkCount > MAX_CHECK_COUNT && doCheck)
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
std::list<Collidable*> Physics::IsCollideLine(const VECTOR& _start, const VECTOR& _end)const
{
	std::list<Collidable*> ret;

	for (auto& object : collidables)
	{
		//collidable�̎�ނɂ���āA�����蔻��𕪂���
		auto kind = object->colliderData->GetKind();
		//���C���ƕ���
		if (kind == ColliderData::Kind::PLANE)
		{
			auto planeColliderData = dynamic_cast<ColliderDataPlane*>(object->colliderData);
			//���C���Ɠ_�̍ŒZ���������߂�
			float minLength = Segment_Point_MinLength(_start, _end, object->rigidbody.GetPosition());
			//float�̌덷���l�����Ĕ���
			if (minLength < 0.001f)
			{
				ret.emplace_back(object);
			}
		}
	}
	return ret;
}

/// <summary>
/// �������Ă��邩�ǂ�����������
/// </summary>
bool Physics::IsCollide(const Collidable* _objectA, const Collidable* _objectB)const
{
	bool isHit = false;

	/*�ǂ��炩�̃X���[�ΏۂƂ��ă^�O�������Ă����疳��*/
	if (_objectA->IsThroughTarget(_objectB) || _objectB->IsThroughTarget(_objectA))
	{
		return false;
	}

	/*collidable�̎�ނɂ���ē����蔻��𕪂���*/
	auto aKind = _objectA->colliderData->GetKind();
	auto bKind = _objectB->colliderData->GetKind();

	/*���Ƌ�*/
	if (aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::SPHERE)
	{
		//auto aTob = VSub(_objectB->nextPosition, _objectA->nextPosition);
		//auto aTobLength = VSize(aTob);

		///*�݂��̋������A���ꂼ��̔��a�̍��v������������Γ�����*/
		//auto objectAColliderData = dynamic_cast<ColliderDataSphere*>(_objectA->colliderData);
		//auto objectBColliderData = dynamic_cast<ColliderDataSphere*>(_objectB->colliderData);
		//isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}
	/*�J�v�Z���ƃJ�v�Z��*/
	else if (aKind == ColliderData::Kind::CAPSULE && bKind == ColliderData::Kind::CAPSULE)
	{
		auto aTob = VSub(_objectB->nextPosition, _objectA->nextPosition);
		auto aTobLength = VSize(aTob);

		/*�݂��̋������A���ꂼ��̔��a�̍��v������������Γ�����*/
		auto objectAColliderData = dynamic_cast<ColliderDataCapsule*>(_objectA->colliderData);
		auto objectBColliderData = dynamic_cast<ColliderDataCapsule*>(_objectB->colliderData);
		isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}

	/*���ƃJ�v�Z��*/
	else if ((aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::CAPSULE) ||
		(aKind == ColliderData::Kind::CAPSULE && bKind == ColliderData::Kind::SPHERE))
	{
		auto aTag = _objectA->GetTag();
		auto bTag = _objectB->GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			ColliderData* sphereDataBase = _objectA->colliderData;
			VECTOR sphereCenter = _objectA->nextPosition;
			ColliderData* capsuleDataBase = _objectB->colliderData;
			VECTOR capsuleUnder = _objectB->nextPosition;
			if (bKind == ColliderData::Kind::SPHERE)
			{
				sphereDataBase = _objectB->colliderData;
				sphereCenter = _objectB->nextPosition;
				capsuleDataBase = _objectA->colliderData;
				capsuleUnder = _objectA->nextPosition;
			}
			auto sphereColliderData = dynamic_cast<ColliderDataSphere*>(sphereDataBase);
			auto capsuleColliderData = dynamic_cast<ColliderDataCapsule*>(capsuleDataBase);
			if (sphereColliderData->GetHitNumber() != capsuleColliderData->GetHitNumber())
			{
				VECTOR capsuleTop = VGet(capsuleUnder.x, capsuleUnder.y + capsuleColliderData->height, capsuleUnder.z);
				isHit = HitCheck_Sphere_Capsule(sphereCenter, sphereColliderData->radius, capsuleUnder, capsuleTop, capsuleColliderData->radius);
			}
		}
	}
	/*�J�v�Z���ƕ��ʂ̓����蔻��*/
	else if ((aKind == ColliderData::Kind::CAPSULE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::CAPSULE))
	{
		ColliderData* planeDataBase = _objectA->colliderData;
		VECTOR planeCenter = _objectA->rigidbody.GetPosition();
		VECTOR capsuleUnder = _objectB->nextPosition;
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = _objectB->colliderData;
			planeCenter = _objectB->nextPosition;
			capsuleUnder = _objectA->nextPosition;
		}
		auto planeColliderData = dynamic_cast<ColliderDataPlane*>(planeDataBase);

		if (bKind == ColliderData::Kind::CAPSULE)
		{
			capsuleUnder = _objectB->nextPosition;
		}
		/*���͒n�ʂ��~�`�̕��ʂ������Ă���̂ŁA�����蔻���Y���W(�O�ȉ����ǂ���)�ƕ��ʂ̒��S���W�����ʂ̔��a���Ȃ����𔻒肷��*/
		float distance = VSize(VSub(capsuleUnder, planeCenter));
		if ((capsuleUnder.y < 0.0f) || (distance > planeColliderData->radius))
		{
			isHit = true;
		}
	}
	return isHit;
}


void Physics::FixNextPosition(Collidable* primary, Collidable* secondary)const
{
	/*�����蔻��̎�ʂ��Ƃɕ␳���@��ς���*/
	auto primaryKind = primary->colliderData->GetKind();
	auto secondaryKind = secondary->colliderData->GetKind();

	//�����m�̈ʒu�␳
	if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		//VECTOR primaryToSecondary = VSub(secondary->nextPosition, primary->nextPosition);
		//VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		//auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary->colliderData);
		//auto secondaryColliderData = dynamic_cast<ColliderDataSphere*> (secondary->colliderData);
		////���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
		//float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		//VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		//VECTOR fixedPosition = VAdd(primary->nextPosition, primaryToNewSecondaryPosition);
		//secondary->nextPosition = fixedPosition;
	}
	//�J�v�Z�����m�̈ʒu�␳
	else if (primaryKind == ColliderData::Kind::CAPSULE && secondaryKind == ColliderData::Kind::CAPSULE)
	{
		VECTOR primaryToSecondary = VSub(secondary->nextPosition, primary->nextPosition);
		VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		auto primaryColliderData = dynamic_cast<ColliderDataCapsule*> (primary->colliderData);
		auto secondaryColliderData = dynamic_cast<ColliderDataCapsule*> (secondary->colliderData);
		//���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
		float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		VECTOR fixedPosition = VAdd(primary->nextPosition, primaryToNewSecondaryPosition);
		secondary->nextPosition = fixedPosition;
	}
	//���ʂƃJ�v�Z��(���ʂ�STATIC�Ȃ̂ŁA�K��primary��PLANE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::CAPSULE)
	{
		auto primaryColliderData = dynamic_cast<ColliderDataPlane*> (primary->colliderData);

		VECTOR fixedPosition = secondary->nextPosition;
		VECTOR secondaryToPrimary = VSub(secondary->nextPosition, primary->rigidbody.GetPosition());
		float distance = VSize(secondaryToPrimary);
		float fixValue = 0.0f;

		if (distance > primaryColliderData->radius)
		{
			fixValue = primaryColliderData->radius - distance;
			fixedPosition = VAdd(fixedPosition, VScale(VNorm(secondaryToPrimary), fixValue));
		}
		fixedPosition.y = 0.0f;
		secondary->nextPosition = fixedPosition;
	}
	//���ƃJ�v�Z��(����STATIC�Ȃ̂ŁA�K��primary��SPHERE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::CAPSULE)
	{
		auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary->colliderData);
		auto secondaryColliderData = dynamic_cast<ColliderDataCapsule*> (secondary->colliderData);
		if (secondaryColliderData->hitNumber != primaryColliderData->hitNumber)
		{
			if (!secondaryColliderData->isCutDamage)
			{
				secondaryColliderData->hp -= primaryColliderData->damage;
			}
			secondaryColliderData->hitNumber = primaryColliderData->hitNumber;
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
void Physics::FixPosition()
{
	for (auto& item : collidables)
	{
		//position���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		VECTOR toFixedPosition = VSub(item->nextPosition, item->rigidbody.GetPosition());
		item->rigidbody.SetVelocity(toFixedPosition);

		//�ʒu�m��
		item->rigidbody.SetPosition(item->nextPosition);
	}
}

void Physics::DamageCalc(Collidable* _primary, Collidable* _secondary)const
{

}