#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
using namespace GoriLib;
#include "BossAttack.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAttack::BossAttack(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger)
	: Collidable			(_priority, _tag, _colliderKind, _isTrigger)
	, isStartHitCheck		(false)
	, frameCount			(0)
	, damage				(0)
	, attackIndex			(0)
	, stackSpeed			(0.0f)
	, basePosition			(VGet(0.0f, 0.0f, 0.0f))
	, direction				(VGet(0.0f, 0.0f, 0.0f))
	, isDontStartPrevFrame	(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossAttack::~BossAttack()
{

}

/// <summary>
/// �㏈��
/// </summary>
void BossAttack::Finalize(GoriLib::Physics* _physics)
{
	/*�����o�^�̉���*/
	Collidable::Finalize(_physics);
}
/// <summary>
/// �Փ˂�����
/// </summary>
void BossAttack::OnCollide(const Collidable& _colider)
{
	//std::string message = "�v���C���[�̍U����";
	//if (_colider.GetTag() == GameObjectTag::BOSS)
	//{
	//	message += "�{�X";
	//}

	//message += "�Ɠ�������\n";
	//printfDx(message.c_str());

}

/// <summary>
/// �`��
/// </summary>
const void BossAttack::Draw()const
{
#if _DEBUG
	if (this->isStartHitCheck)
	{
		DrawSphere3D(this->rigidbody.GetPosition(), this->GetRadius(), 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->rigidbody.GetPosition();
	printfDx("MAIN_1_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
#endif // _DEBUG
}

/// <summary>
/// ���W�̎擾
/// </summary>
const VECTOR BossAttack::GetPosition()const
{
	return this->rigidbody.GetPosition();
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
