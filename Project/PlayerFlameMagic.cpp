#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerFlameMagic.h"
#include "PlayerManager.h"
#include "CameraManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerFlameMagic::PlayerFlameMagic()
	: endPosition{ 0.0f,0.0f,0.0f }
	, frameCount(0)
	, range(0.0f)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerFlameMagic::~PlayerFlameMagic()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerFlameMagic::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();

	/*�g�p����ϐ���������*/
	const float POSITION_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_POSITON_OFFSET"];
	this->moveVector			= camera.GetCameraDirection();
	this->moveVector			= VNorm(this->moveVector);
	this->position				= player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->radius				= json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_RADIUS"];
	this->range					= json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_RANGE"];
	this->moveVector			= VScale(this->moveVector, -this->range);
	this->endPosition			= this->position + this->moveVector;
	this->isInitialize			= true;
	this->frameCount			= 0;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerFlameMagic::Update()
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
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_INTERVAL"];
	this->frameCount++;
	
	/*�t���[�����ő�l��������e�t���O������*/
	if (this->frameCount >= MAX_FRAME_COUNT)
	{
		this->isInitialize = false;
		this->isStart = false;
		this->frameCount = 0;
	}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerFlameMagic::Draw()const
{
	/*���������Ă��Ȃ�������return��Ԃ�*/
	if (!this->isInitialize)return;

	DrawCapsule3D(this->position, this->endPosition, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}