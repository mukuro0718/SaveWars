#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CollisionManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ColliderData::ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag)
	: kind			(_kind)
	, isHit			(false)
	, nextPositon	(VGet(0.0f, 0.0f, 0.0f))
	, tag			(_tag)
	, priority		(_priority)
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& manager = Singleton<CollisionManager>::GetInstance();
	
	/*�}�l�[�W���[�ɓo�^*/
	manager.Entry(*this);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ColliderData::~ColliderData()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& manager = Singleton<CollisionManager>::GetInstance();

	/*�}�l�[�W���[����폜*/
	manager.Exit(*this);
}