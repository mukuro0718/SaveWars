#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerIceMagic.h"
#include "CameraManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerIceMagic::PlayerIceMagic()
	: firstPosition{ 0.0f,0.0f,0.0f }
	, velocity(0.0f)
	, isBooting(false)
	, frameCount(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerIceMagic::~PlayerIceMagic()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerIceMagic::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�萔�^�̏����l���쐬*/
	const float POSITION_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_POSITON_OFFSET"];
	const float RADIUS			= json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_RADIUS"];
	const float VELOCITY		= json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_VELOCITY"];
	/*�ϐ���������*/
	this->moveVector = VScale(camera.GetCameraDirection(), -1.0f);
	this->moveVector = VNorm(this->moveVector);
	this->position = player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->firstPosition = this->position;
	this->radius = RADIUS;
	this->velocity = VELOCITY;
	this->moveVector = VScale(this->moveVector, this->velocity);
	this->isInitialize = true;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerIceMagic::Update()
{
	/*�������@���J�n���Ă��Ȃ������瑁�����^�[��*/
	if (!this->isStart)return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*���������Ă��Ȃ�������*/
	if (!this->isInitialize)
	{
		Initialize();
	}
	/*�t���[���v��*/
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_INTERVAL"];
	this->frameCount++;

	/*���@���N�����Ă��Ȃ��Ƃ��t���[�����ő�l�ȏゾ�����疂�@���N������*/
	if (!this->isBooting && this->frameCount >= MAX_FRAME_COUNT)
	{
		this->isBooting = true;
		this->frameCount = 0;
	}

	/*���@���N�����Ă��Ȃ������瑁�����^�[��*/
	if (!this->isBooting)return;

	/*���W�̍X�V*/
	this->position = this->position + this->moveVector;

	/*�������W�Ƃ̋������萔�ȏゾ������e�t���O������*/
	VECTOR		firstToNowPositionVector = this->position - this->firstPosition;
	float		firstToNowPositionVectorSize = VSize(firstToNowPositionVector);
	const float MAX_MOVE_RANGE = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_MAX_MOVE_RANGE"];
	if (firstToNowPositionVectorSize >= MAX_MOVE_RANGE)
	{
		this->isStart = false;
		this->isInitialize = false;
		this->isBooting = false;
		this->frameCount = 0;
	}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerIceMagic::Draw()const
{
	/*�ړ����Ă��Ȃ�������return��Ԃ�*/
	if (!this->isStart)return;
	VECTOR endPosition = this->position;
	endPosition.y -= this->radius;
	DrawCapsule3D(this->position, endPosition,this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}