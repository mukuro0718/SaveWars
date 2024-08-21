#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerEarthMagic.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerEarthMagic::PlayerEarthMagic()
	: frameCount(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerEarthMagic::~PlayerEarthMagic()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerEarthMagic::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�萔�^�̏����l���쐬*/
	const float RADIUS = json.GetJson(JsonManager::FileType::PLAYER)["EARTH_MAGIC_RADIUS"];
	/*�ϐ���������*/
	this->position = player.GetPosition();
	this->radius = RADIUS;
	this->frameCount = 0;
	this->isInitialize = true;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerEarthMagic::Update()
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
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["EARTH_MAGIC_INTERVAL"];
	this->frameCount++;

	/*�t���[�����ő�l�ȏゾ������e�t���O������*/
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
const void PlayerEarthMagic::Draw()const
{
	/*�ړ����Ă��Ȃ�������return��Ԃ�*/
	if (!this->isInitialize)return;

	DrawSphere3D(this->position, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}