#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossStabAttack.h"
#include "EnemyManager.h"
#include "Debug.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossStabAttack::BossStabAttack(const int _attackIndex)
	: BossAttack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*���̍U�����{�X�̉��Ԗڂ̍U������ۑ����Ă���*/
	this->attackIndex = _attackIndex;

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, new AttackData());

	/*�R���C�_�[�̏�����*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	collider.data->damage = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	collider.data->reactionType = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	//�����ł̃q�b�g�X�g�b�v�n�̕ϐ��́A�L�����N�^�[���ɗ^������̂ɂȂ�
	collider.data->hitStopTime = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	collider.data->hitStopType = static_cast<int>(HitStop::Type::STOP);
	collider.data->hitStopDelay = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	collider.data->slowFactor = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];

	this->startHitCheckFrame = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"][this->attackIndex];
	this->endHitCheckFrame = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"][this->attackIndex];
	this->positionOffset = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"][this->attackIndex];
	this->yOffset = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][this->attackIndex];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossStabAttack::~BossStabAttack()
{

}

/// <summary>
/// ������
/// </summary>
void BossStabAttack::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�̏�����*/
	auto& collider	= dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.data->isHitAttack	= false;

	/*�ϐ��̏�����*/
	this->frameCount	  = 0;
	this->isStartHitCheck = false;
	this->isStartHitCheck = false;
	this->isNotOnHit	  = false;

	/*���������̏�����*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// �X�V
/// </summary>
void BossStabAttack::Update(const float _playTime)
{

	/*�����蔻��̊m�F���J�n���Ă���*/
	if (this->isStartHitCheck)
	{
		//�t���[���𑝂₷
		this->frameCount++;
		//�t���[�����萔�𒴂��Ă��Ȃ������瑁�����^�[��
		if (this->frameCount < this->startHitCheckFrame)return;

		auto& enemy = Singleton<EnemyManager>::GetInstance();
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		//����̍U�����ɓ����蔻��t���O����x�������Ă��Ȃ�������
		if (!this->isNotOnHit)
		{
			collider.data->isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//�����蔻��ʒu�̍X�V
		collider.rigidbody.SetPosition(MV1GetFramePosition(enemy.GetModelHandle(), 9));
		collider.topPositon = MV1GetFramePosition(enemy.GetModelHandle(), 11);
		//�t���[�����萔�𒴂��Ă����瓖���蔻��J�n�t���O������
		if (this->frameCount > this->endHitCheckFrame || (this->isNotOnHit && !collider.data->isDoHitCheck))
		{
			this->isStartHitCheck = false;
			collider.data->isDoHitCheck = false;
			this->frameCount = 0;
			collider.data->isHitAttack = false;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void BossStabAttack::Draw()const
{
	if (this->isStartHitCheck)
	{
		auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
		DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = this->collider->rigidbody.GetPosition();
	printfDx("JUMP_ATTACK X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
}