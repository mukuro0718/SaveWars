#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerStormMagic.h"
#include "CameraManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerStormMagic::PlayerStormMagic()
	: endPosition{ 0.0f,0.0f,0.0f }
	, velocity(0.0f)
	, frameCount(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStormMagic::~PlayerStormMagic()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerStormMagic::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�萔�^�̏����l���쐬*/
	const float POSITION_OFFSET   = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_POSITON_OFFSET"];
	const float POSITION_OFFSET_Y = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_POSITON_Y_OFFSET"];
	const float RADIUS			  = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_RADIUS"];
	/*�ϐ���������*/
	this->moveVector	=  VScale(camera.GetCameraDirection(), -1.0f);
	this->moveVector	=  VNorm(this->moveVector);
	this->position		=  player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->endPosition	=  this->position;
	this->endPosition.y += POSITION_OFFSET_Y;
	this->radius		=  RADIUS;
	this->frameCount	=  0;
	this->isInitialize	=  true;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStormMagic::Update()
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
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_INTERVAL"];
	this->frameCount++;

	if (this->frameCount >= MAX_FRAME_COUNT)
	{
		this->isStart = false;
		this->isInitialize = false;
		this->frameCount = 0;
	}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerStormMagic::Draw()const
{
	/*�ړ����Ă��Ȃ�������return��Ԃ�*/
	if (!this->isStart)return;

	DrawCapsule3D(this->position, this->endPosition, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}