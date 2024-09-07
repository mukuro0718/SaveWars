#include <cassert>
#include <DxLib.h>
#include "UseJson.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
using namespace GoriLib;
#include "BossAttack.h"
#include "BossFlyAttack.h"
#include "EnemyManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
BossFlyAttack::BossFlyAttack(const int _attackIndex)
	: BossAttack(Collidable::Priority::STATIC, GameObjectTag::BOSS_ATTACK, GoriLib::ColliderData::Kind::SPHERE, false)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*���̍U�����{�X�̉��Ԗڂ̍U������ۑ����Ă���*/
	this->attackIndex = _attackIndex;

	/*�R���C�_�[�f�[�^�̍쐬*/
	auto sphereColiderData		 = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius	 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	sphereColiderData->damage	 = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	sphereColiderData->hitNumber = 0;

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
void BossFlyAttack::Initialize(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�̏�����*/
	Collidable::Initialize(_physics);

	/*�ϐ��̏�����*/
	this->frameCount		   = 0;
	this->stackSpeed		   = 0.0f;
	this->isStartHitCheck	   = false;
	this->basePosition		   = Convert(json.GetJson(JsonManager::FileType::ENEMY)["OUT_POSITION"]);
	this->direction			   = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
	this->isStartHitCheck	   = false;
	this->isDontStartPrevFrame = false;

	/*���������̏�����*/
	this->rigidbody.Initialize(false);
	this->rigidbody.SetPosition(this->basePosition);

	/*�R���C�_�[�f�[�^�̏�����*/
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	sphereColiderData->damage = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];

}
/// <summary>
/// �X�V
/// </summary>
void BossFlyAttack::Update(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*�����蔻��̊m�F���J�n���Ă���*/
	if (this->isStartHitCheck)
	{
		//�O�̃t���[���œ����蔻����s���Ă��Ȃ�������
		if (!this->isDontStartPrevFrame)
		{
			auto sphereColiderData		 = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
			sphereColiderData->hitNumber = enemy.GetHitNumber();
			this->isDontStartPrevFrame	 = true;
			this->stackSpeed			 = 0.0f;
		}
		//�ϐ��̏���
		const int	START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"]	[this->attackIndex];
		const int	END_HIT_CHECK_FRAME	  = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"]	[this->attackIndex];
		const float POSITION_OFFSET		  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"]			[this->attackIndex];
		const float Y_OFFSET			  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"]		[this->attackIndex];

		//�t���[���𑝂₷
		this->frameCount++;
		//�t���[�����萔�𒴂��Ă��Ȃ������瑁�����^�[��
		if (this->frameCount < START_HIT_CHECK_FRAME)return;
		//�����蔻��ʒu�̍X�V
		VECTOR position = MV1GetFramePosition(enemy.GetModelHandle(), 6);
		this->rigidbody.SetPosition(position);
		//�t���[�����萔�𒴂��Ă����瓖���蔻��J�n�t���O������
		if (this->frameCount > END_HIT_CHECK_FRAME)
		{
			this->isStartHitCheck = false;
			this->frameCount = 0;
		}
	}
	else
	{
		//�����蔻��̍��W�̃Z�b�g
		this->isDontStartPrevFrame = false;
		this->direction			   = enemy.GetDirection();
		this->basePosition		   = Convert(json.GetJson(JsonManager::FileType::ENEMY)["OUT_POSITION"]);
		this->rigidbody.SetPosition(this->basePosition);
	}
}

/// <summary>
/// �`��
/// </summary>
const void BossFlyAttack::Draw()const
{
#if _DEBUG
	if (this->isStartHitCheck)
	{
		DrawSphere3D(this->rigidbody.GetPosition(), this->GetRadius(), 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = rigidbody.GetPosition();
	printfDx("MAIN_1_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
#endif // _DEBUG
}


const float BossFlyAttack::GetRadius()const
{
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	return sphereColiderData->radius;
}

void BossFlyAttack::SetRadius(const float _radius)
{
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = _radius;
}