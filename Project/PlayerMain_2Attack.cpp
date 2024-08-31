#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "PlayerMain_2Attack.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerMain_2Attack::PlayerMain_2Attack()
	: Collidable(Collidable::Priority::STATIC, GameObjectTag::PLAYER_ATTACK, GoriLib::ColliderData::Kind::SPHERE, false)
	, isStartHitCheck(false)
	, frameCount(0)
	, damage(0)
	, radius(0.0f)
	, basePosition(VGet(0.0f, 0.0f, 0.0f))
	, direction(VGet(0.0f, 0.0f, 0.0f))
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->radius = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_RADIUS"][this->ATTACK_NUM];
	this->damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"][this->ATTACK_NUM];
	/*�R���C�_�[�f�[�^�̍쐬*/
	auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
	sphereColiderData->radius = this->radius;
	sphereColiderData->damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"][this->ATTACK_NUM];
	sphereColiderData->hitNumber = 0;

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerMain_2Attack::~PlayerMain_2Attack()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerMain_2Attack::Initialize(GoriLib::Physics* _physics)
{
	/*�R���C�_�[�̏�����*/
	Collidable::Initialize(_physics);

	/*���������̏�����*/
	this->rigidbody.Initialize(false);
	this->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
	this->isStartHitCheck = false;
	this->frameCount = 0;
	this->basePosition = VGet(0.0f, 500.0f, 0.0f);
	this->direction = VGet(0.0f, 0.0f, 0.0f);
	this->isDontStartPrevFrame = false;
}
/// <summary>
/// �㏈��
/// </summary>
void PlayerMain_2Attack::Finalize(GoriLib::Physics* _physics)
{
	/*�����o�^�̉���*/
	Collidable::Finalize(_physics);
}
/// <summary>
/// �X�V
/// </summary>
void PlayerMain_2Attack::Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�����蔻��̊m�F���J�n���Ă���*/
	if (this->isStartHitCheck)
	{
		if (!this->isDontStartPrevFrame)
		{
			auto sphereColiderData = dynamic_cast<GoriLib::ColliderDataSphere*>(this->colliderData);
			sphereColiderData->hitNumber = player.GetHitNumber();
			this->isDontStartPrevFrame = true;
		}

		//�ϐ��̏���
		const int START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::PLAYER)["START_HIT_CHECK_FRAME"][this->ATTACK_NUM];
		const int END_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::PLAYER)["END_HIT_CHECK_FRAME"][this->ATTACK_NUM];
		const float POSITION_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"][this->ATTACK_NUM];
		const float Y_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"][this->ATTACK_NUM];
		VECTOR position = VScale(this->direction, POSITION_OFFSET);
		position.y += Y_OFFSET;
		position = VAdd(this->basePosition, position);

		//�t���[���𑝂₷
		this->frameCount++;
		//�t���[�����萔�𒴂��Ă��Ȃ������瑁�����^�[��
		if (this->frameCount < START_HIT_CHECK_FRAME)return;
		//�����蔻��̍��W�̃Z�b�g
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
		this->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
		this->basePosition = _position;
		this->direction = _direction;
		this->isDontStartPrevFrame = false;
	}
}
/// <summary>
/// �Փ˂�����
/// </summary>
void PlayerMain_2Attack::OnCollide(const Collidable& _colider)
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
const void PlayerMain_2Attack::Draw()const
{
#if _DEBUG
	if (this->isStartHitCheck)
	{
		DrawSphere3D(this->rigidbody.GetPosition(), this->radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
	}
	VECTOR position = rigidbody.GetPosition();
		printfDx("MAIN_2_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
#endif // _DEBUG
}
