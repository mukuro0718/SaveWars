#include <cassert>
#include <DxLib.h>
#include "DeleteInstance.h"
#include "GoriLib.h"
using namespace GoriLib;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Collidable::Collidable(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger)
	: priority(_priority)
	, tag(_tag)
	, colliderData(nullptr)
	, nextPosition(VGet(0.0f, 0.0f, 0.0f))
{
	CreateColliderData(_colliderKind, _isTrigger);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Collidable::~Collidable()
{
	DeleteMemberInstance(this->colliderData);
}

/// <summary>
/// ������
/// </summary>
void Collidable::Initialize(GoriLib::Physics* _physics)
{
	_physics->Entry(this);
}

/// <summary>
/// �I��
/// </summary>
void Collidable::Finalize(GoriLib::Physics* _physics)
{
	_physics->Exit(this);
}

/// <summary>
/// �����蔻��𖳎�����^�O�̒ǉ�
/// </summary>
/// <param name="_tag"></param>
void Collidable::AddThroughTag(GameObjectTag _tag)
{
	bool found = (std::find(this->throughTags.begin(), this->throughTags.end(), this->tag) != this->throughTags.end());
	if (found)
	{
		assert(0 && "�w��^�O�͊��ɒǉ�����Ă��܂��B");
	}
	else
	{
		this->throughTags.emplace_back(this->tag);
	}
}

/// <summary>
/// �����蔻��𖳎�����^�O�̍폜
/// </summary>
/// <param name="_tag"></param>
void Collidable::RemoveThroughTag(GameObjectTag _tag)
{
	bool found = (std::find(this->throughTags.begin(), this->throughTags.end(), this->tag) != this->throughTags.end());
	if (found)
	{
		assert(0 && "�w��^�O�͑��݂��܂���B");
	}
	else
	{
		this->throughTags.remove(this->tag);
	}
}

/// <summary>
/// �����蔻��𖳎�����Ώۂ��ǂ���
/// </summary>
bool Collidable::IsThroughTarget(const Collidable* _target)const
{
	bool found = (std::find(this->throughTags.begin(), this->throughTags.end(), this->tag) != this->throughTags.end());
	return found;
}

/// <summary>
/// �����蔻��f�[�^�̍쐬
/// </summary>
ColliderData* Collidable::CreateColliderData(ColliderData::Kind _kind, bool _isTrigger)
{
	if (this->colliderData != nullptr)
	{
		assert(0 && "colliderData�͊��ɍ���Ă��܂�");
		return this->colliderData;
	}

	switch (_kind)
	{
	case GoriLib::ColliderData::Kind::SPHERE:
		this->colliderData = new ColliderDataSphere(_isTrigger);
		break;
	case GoriLib::ColliderData::Kind::CAPSULE:
		this->colliderData = new ColliderDataCapsule(_isTrigger);
		break;
	case GoriLib::ColliderData::Kind::PLANE:
		this->colliderData = new ColliderDataPlane(_isTrigger);
		break;
	//case GoriLib::ColliderData::Kind::CUBE:
	//	break;
	//default:
	//	break;
	}
	return this->colliderData;
}