#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "PlayerAttackData.h"
#include "AttackSphereColliderData.h"
#include "PlayerAttack.h"
#include "Debug.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerAttack::PlayerAttack()
	: isStartHitCheck(false)
	, frameCount(0)
	, collider(nullptr)
	, isNotOnHit(false)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�f�[�^�̍쐬*/
	AttackData* data = new PlayerAttackData();
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER_ATTACK, data);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerAttack::~PlayerAttack()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerAttack::Initialize()
{	
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�̏�����*/
	auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerAttackData&>(*collider.data);
	collider.radius = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_RADIUS"];
	data.damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"];
	data.hitStopTime = json.GetJson(JsonManager::FileType::PLAYER)["HIT_STOP_TIME"];

	/*���������̏�����*/
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
	this->isStartHitCheck = false;
	this->frameCount = 0;
	this->isNotOnHit = false;
}
/// <summary>
/// �㏈��
/// </summary>
void PlayerAttack::Finalize()
{
}
/// <summary>
/// �X�V
/// </summary>
void PlayerAttack::Update(const VECTOR _position, const VECTOR _direction)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	//auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerAttackData&>(*collider.data);

	/*�����蔻��̊m�F���J�n���Ă���*/
	if (this->isStartHitCheck)
	{
		//�ϐ��̏���
		const int	 START_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::PLAYER)["START_HIT_CHECK_FRAME"];//�����蔻��J�n�t���[��
		const int	 END_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::PLAYER)["END_HIT_CHECK_FRAME"];	//�����蔻��I���t���[��
		const float  POSITION_OFFSET		= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"];		//�����蔻����W�I�t�Z�b�g
		const float  Y_OFFSET				= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"];		//Y���W�p�I�t�Z�b�g
		const VECTOR DIRECTION				= _direction;																//�v���C���[�̌���
		VECTOR		position				= _position;										//�v���C���[�̍��W
		position							= VAdd(position, VScale(DIRECTION, POSITION_OFFSET));			//�v���C���[�̍��W�ɁA�I�t�Z�b�g�l�𑫂�
		position.y						   += Y_OFFSET;																	//Y���W�I�t�Z�b�g�l�𑫂�

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

		//�����蔻��̍��W�̃Z�b�g
		this->collider->rigidbody.SetPosition(position);
		//�t���[�����萔�𒴂��Ă���A�����蔻��t���O���~��Ă����瓖���蔻��J�n�t���O������
		if (this->frameCount > END_HIT_CHECK_FRAME || (this->isNotOnHit && !data.isDoHitCheck))
		{
			this->isStartHitCheck = false;
			data.isDoHitCheck = false;
			this->frameCount = 0;
		}
	}
	//else
	//{
	//	//�����蔻��̍��W�̃Z�b�g
	//	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
	//}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerAttack::Draw()const
{
	auto& debug = Singleton<Debug>::GetInstance();

	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER))
	{
		if (this->isStartHitCheck)
		{
			auto& collider = dynamic_cast<AttackSphereColliderData&>(*this->collider);
			DrawSphere3D(this->collider->rigidbody.GetPosition(), collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
		}
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("PLAYER_ATTACK X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	}
}
