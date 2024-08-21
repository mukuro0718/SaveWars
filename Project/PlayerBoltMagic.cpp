#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerBoltMagic.h"
#include "CameraManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerBoltMagic::PlayerBoltMagic()
	: endPosition{ 0.0f,0.0f,0.0f }
	, velocity(0.0f)
	, frameCount()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerBoltMagic::~PlayerBoltMagic()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerBoltMagic::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�萔�^�̏����l���쐬*/
	const float POSITION_OFFSET	  = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_POSITON_OFFSET"];
	const float POSITION_OFFSET_Y = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_POSITON_Y_OFFSET"];
	const float RADIUS			  = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_RADIUS"];
	const float RANGE_OFFSET_Y	  = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_RANGE_BEFORE_OFFSET"];
	/*�ϐ���������*/
	this->moveVector	=  VScale(camera.GetCameraDirection(), -1.0f);
	this->moveVector	=  VNorm(this->moveVector);
	this->position		=  player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->position.y	+= POSITION_OFFSET_Y;
	this->endPosition	=  this->position;
	this->endPosition.y -= RANGE_OFFSET_Y;
	this->radius		=  RADIUS;
	this->frameCount	=  0;
	this->isInitialize	=  true;
	this->isBooting		= false;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerBoltMagic::Update()
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
	int BEFORE_MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_BEFORE_INTERVAL"];
	int AFTER_MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_AFTER_INTERVAL"];
	this->frameCount++;

	/*���@���N�����Ă��Ȃ��Ƃ��t���[�����ő�l�ȏゾ�����疂�@���N������*/
	if (!this->isBooting && this->frameCount >= BEFORE_MAX_FRAME_COUNT)
	{
		this->isBooting = true;
		this->frameCount = 0;
	}

	/*���@���N�����Ă��Ȃ������瑁�����^�[��*/
	if (!this->isBooting)return;

	/*�����蔻��p�̃J�v�Z���̏I�_��L�΂�*/
	const float RANGE_OFFSET_Y = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_RANGE_AFTER_OFFSET"];
	this->endPosition = this->position;
	this->endPosition.y -= RANGE_OFFSET_Y;
	
	if (this->frameCount >= AFTER_MAX_FRAME_COUNT)
	{
		this->isInitialize = false;
		this->isBooting = false;
		this->frameCount = 0;
		this->isStart = false;
	}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerBoltMagic::Draw()const
{
	/*�ړ����Ă��Ȃ�������return��Ԃ�*/
	if (!this->isInitialize)return;

	DrawCapsule3D(this->position, this->endPosition, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}