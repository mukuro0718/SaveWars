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
#include "BossAttackData.h"
#include "AttackCapsuleColliderData.h"
#include "BossAttack.h"
#include "BossFlyAttack.h"
#include "EnemyManager.h"
#include "Debug.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossFlyAttack::BossFlyAttack(const int _attackIndex)
	: BossAttack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*���̍U�����{�X�̉��Ԗڂ̍U������ۑ����Ă���*/
	this->attackIndex = _attackIndex;

	/*�R���C�_�[�f�[�^�̍쐬*/
	AttackData* data = new BossAttackData();
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, data);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossFlyAttack::~BossFlyAttack()
{

}

/// <summary>
/// ������
/// </summary>
void BossFlyAttack::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�̏�����*/
	auto& collider	= dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	auto& data		= dynamic_cast<BossAttackData&>(*collider.data);
	collider.radius		= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	data.damage			= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	data.playerReaction = static_cast<int>(Gori::PlayerReactionType::BLOW_BIG);
	//�����ł̃q�b�g�X�g�b�v�n�̕ϐ��́A�L�����N�^�[���ɗ^������̂ɂȂ�
	data.hitStopTime	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	data.hitStopType	= static_cast<int>(HitStop::Type::STOP);
	data.hitStopDelay	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	data.slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	data.isHitAttack	= false;

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
void BossFlyAttack::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json	  = Singleton<JsonManager>::GetInstance();
	auto& enemy   = Singleton<EnemyManager>::GetInstance();
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	auto& data	  = dynamic_cast<BossAttackData&>(*collider.data);

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
			data.isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//�����蔻��ʒu�̍X�V
		collider.rigidbody.SetPosition(MV1GetFramePosition(enemy.GetModelHandle(), 6));
		collider.topPositon = MV1GetFramePosition(enemy.GetModelHandle(), 1);
		//�t���[�����萔�𒴂��Ă���A�����蔻��t���O���~��Ă����瓖���蔻��J�n�t���O������
		if (this->frameCount > END_HIT_CHECK_FRAME || (this->isNotOnHit && !data.isDoHitCheck))
		{
			this->isStartHitCheck = false;
			data.isDoHitCheck = false;
			this->frameCount = 0;
			data.isHitAttack = false;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void BossFlyAttack::Draw()const
{
	auto& debug = Singleton<Debug>::GetInstance();
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	{
		if (this->isStartHitCheck)
		{
			auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);

			DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
		}
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("FLY_ATTACK X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	}
}