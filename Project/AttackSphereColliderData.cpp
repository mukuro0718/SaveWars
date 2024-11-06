#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
AttackSphereColliderData::AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data)
	: ColliderData	(ColliderData::Kind::ATTACK_SPHERE, _priority, _tag)
	, radius		(0.0f)
	, data			(_data)
{
	//�����Ȃ�
}

/// <summary>
/// �q�b�g����
/// </summary>
void AttackSphereColliderData::OnHit(const CharacterData& _data)
{
	//�U���q�b�g�`�F�b�N�t���O������
	this->data->isDoHitCheck = false;

	/*���G�t���O���K�[�h�t���O�������Ă��Ȃ�������*/
	if (!_data.isInvinvible && !_data.isGuard)
	{
		//�U���ɓ��������t���O�𗧂Ă�
		this->data->isHitAttack = true;
	}
}