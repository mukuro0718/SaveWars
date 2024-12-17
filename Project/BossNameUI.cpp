#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "BossNameUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossNameUI::BossNameUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);
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

}

/// <summary>
/// �`��
/// </summary>
const void BossNameUI::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	int enemyType = enemyChanger.GetEnemyType();
	vector<int>	 position = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_POSITION"][enemyType];
	vector<int> backGround = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_BACKGROUND"][enemyType];
	string name = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_TYPE"][enemyType];
	DrawExtendGraph(backGround[0], backGround[1], backGround[2], backGround[3], this->imageHandle, TRUE);
	DrawStringToHandle(position[0], position[1], name.c_str(), this->TEXT_COLOR, this->fontHandle);
}

