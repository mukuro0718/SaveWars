#include <DxLib.h>
#include "UseSTL.h"
#include "PlayerBoltMagic.h"
#include "PlayerEarthMagic.h"
#include "PlayerFlameMagic.h"
#include "PlayerIceMagic.h"
#include "PlayerStormMagic.h"
#include "PlayerMagicManager.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerMagicManager::PlayerMagicManager()
{
	this->magic[static_cast<int>(Element::FRAME)] = new PlayerFlameMagic();
	this->magic[static_cast<int>(Element::BOLT)]  = new PlayerBoltMagic ();
	this->magic[static_cast<int>(Element::ICE)]   = new PlayerIceMagic  ();
	this->magic[static_cast<int>(Element::EARTH)] = new PlayerEarthMagic();
	this->magic[static_cast<int>(Element::STORM)] = new PlayerStormMagic();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerMagicManager::~PlayerMagicManager()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerMagicManager::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	//auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�v���C���[�̍��̑������擾*/
	int currentElement = player.GetCurrentElement();

	/*�v���C���[���U�����Ă�����*/
	if (player.IsAttack())
	{
		this->magic[currentElement]->OnIsStart();
	}

	for (int i = 0; i < this->MAGIC_NUM; i++)
	{
		this->magic[i]->Update();
	}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerMagicManager::Draw()const
{
	for (int i = 0; i < this->MAGIC_NUM; i++)
	{
		this->magic[i]->Draw();
	}
}