#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ColliderData.h"
#include "HitStop.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"
#include "BossAttack.h"
#include "BossJumpAttack.h"
#include "EnemyManager.h"
#include "Debug.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossJumpAttack::BossJumpAttack(const int _attackIndex)
	: BossAttack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*���̍U�����{�X�̉��Ԗڂ̍U������ۑ����Ă���*/
	this->attackIndex = _attackIndex;

	/*�R���C�_�[�f�[�^�̍쐬*/
	AttackData* data = new AttackData();
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, data);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossJumpAttack::~BossJumpAttack()
{

}

/// <summary>
/// ������
/// </summary>
void BossJumpAttack::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�̏�����*/
	auto& collider	= dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.radius		= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	collider.data->damage			= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	collider.data->reactionType = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	//�����ł̃q�b�g�X�g�b�v�n�̕ϐ��́A�L�����N�^�[���ɗ^������̂ɂȂ�
	collider.data->hitStopTime	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	collider.data->hitStopType	= static_cast<int>(HitStop::Type::STOP);
	collider.data->hitStopDelay	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	collider.data->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	collider.data->isHitAttack	= false;

	/*�ϐ��̏�����*/
	this->frameCount	  = 0;
	this->isStartHitCheck = false;
	this->isNotOnHit	  = false;

	/*���������̏�����*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// �X�V
/// </summary>
void BossJumpAttack::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json	  = Singleton<JsonManager>::GetInstance();
	auto& enemy   = Singleton<EnemyManager>::GetInstance();
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

	/*�����蔻��̊m�F���J�n���Ă���*/
	if (this->isStartHitCheck)
	{
		//�ϐ��̏���
		const int	START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"]	[this->attackIndex];
		const int	END_HIT_CHECK_FRAME	  = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"]		[this->attackIndex];
		const float POSITION_OFFSET		  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"]			[this->attackIndex];
		const float Y_OFFSET			  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"]			[this->attackIndex];

		//�t���[���𑝂₷
		this->frameCount++;
		//�t���[�����萔�𒴂��Ă��Ȃ������瑁�����^�[��
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		//����̍U�����ɓ����蔻��t���O����x�������Ă��Ȃ�������
		if (!this->isNotOnHit)
		{
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//���̍��W
		VECTOR headPosition = MV1GetFramePosition(enemy.GetModelHandle(), 6);
		//�K�̍��W
		VECTOR hipPosition = MV1GetFramePosition(enemy.GetModelHandle(), 1);
		//������K�֐L�т�x�N�g��
		VECTOR headToHipVector = VNorm(VSub(hipPosition, headPosition));
		//������K�֐L�т�x�N�g����萔�ŃX�P�[�����O�������̂𓪂̍��W�ɑ��������̂��J�v�Z�������W�Ƃ���
		VECTOR underPosition = VScale(headToHipVector, json.GetJson(JsonManager::FileType::ENEMY)["BACK_BORN_SIZE"]);
		underPosition = VAdd(underPosition, headPosition);
		//�Ђ��̍��W���J�v�Z�������W�Ƃ���
		collider.rigidbody.SetPosition(underPosition);
		//�ܐ�̍��W���J�v�Z������W�Ƃ���
		collider.topPositon = headPosition;

		//�t���[�����萔�𒴂��Ă���A�����蔻��t���O���~��Ă����瓖���蔻��J�n�t���O������
		if (this->frameCount > END_HIT_CHECK_FRAME || (this->isNotOnHit && !collider.data->isDoHitCheck))
		{
			this->isStartHitCheck = false;
			collider.data->isDoHitCheck	  = false;
			this->frameCount	  = 0;
			collider.data->isHitAttack	  = false;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void BossJumpAttack::Draw()const
{
	if (this->isStartHitCheck)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

		DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->collider->rigidbody.GetPosition();
	printfDx("BossJumpAttack X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
}