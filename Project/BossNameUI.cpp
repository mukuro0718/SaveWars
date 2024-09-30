#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "BossNameUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossNameUI::BossNameUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_150_64));
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_70_64));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossNameUI::~BossNameUI()
{

}

/// <summary>
/// ������
/// </summary>
void BossNameUI::Initialize()
{
	this->positionType = 0;
	this->drawTextIndex = 0;
	this->addTextIndexInterval = 0;
	this->entryInterval = 0;
}

/// <summary>
/// �X�V
/// </summary>
void BossNameUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto&  enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*���W�̎�ނ̕ύX*/
	ChangePositionType();

	/*�e�L�X�g�C���f�b�N�X�̒ǉ�*/
	AddTextIndex();
}

/// <summary>
/// �`��
/// </summary>
const void BossNameUI::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	const vector<string> TEXT	  = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME"];
	const vector<int>	 OFFSET   = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_OFFSET"][this->positionType];
		  vector<int>	 position = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_POSITION"][this->positionType];
	for (int i = 0; i < this->drawTextIndex; i++)
	{
		DrawStringToHandle(position[0], position[1], TEXT[i].c_str(), this->TEXT_COLOR, this->fontHandle[this->positionType]);
		position[0] += OFFSET[i];
	}
}

/// <summary>
/// �e�L�X�g�C���f�b�N�X�̒ǉ�
/// </summary>
void BossNameUI::AddTextIndex()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	
	if (player.GetIsAlive())
	{
		if (this->drawTextIndex < json.GetJson(JsonManager::FileType::UI)["BOSS_NAME"].size())
		{
			this->addTextIndexInterval++;
			if (this->addTextIndexInterval >= json.GetJson(JsonManager::FileType::UI)["TEXT_INDEX_INTERVAL"])
			{
				this->addTextIndexInterval = 0;
				this->drawTextIndex++;
			}
		}
	}
}

/// <summary>
/// ���W�̎�ނ̕ύX
/// </summary>
void BossNameUI::ChangePositionType()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	if (player.GetIsAlive())
	{
		if (this->entryInterval < json.GetJson(JsonManager::FileType::CAMERA)["ENTRY_CAMERA_INTERVAL"])
		{
			this->positionType = static_cast<int>(PositionType::ENTRY);
			this->entryInterval++;
		}
		else
		{
			this->positionType = static_cast<int>(PositionType::NORMAL);
		}
	}
}