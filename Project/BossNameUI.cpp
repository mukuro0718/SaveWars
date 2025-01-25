#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "BossNameUI.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossNameUI::BossNameUI()
	: fontHandle(-1)
	, imageHandle(-1)
{
	/*�摜�n���h���̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->fontHandle  = asset.GetFont(LoadingAsset::FontType::BOSS_NAME);
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
	/*�ϐ��̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	int			 enemyType	 = enemyChanger.GetEnemyType();
	vector<int>	 position	 = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_POSITION"][enemyType];
	vector<int>  background  = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_BACKGROUND"][enemyType];
	string		 name		 = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_TYPE"][enemyType];
	this->namePosition		 = position;
	this->backgroundDrawRect = background;
	this->bossName			 = name;
}

/// <summary>
/// �X�V
/// </summary>
void BossNameUI::Update()
{
}

/// <summary>
/// �`��
/// </summary>
const void BossNameUI::Draw()const
{
	//DrawExtendGraph(this->backgroundDrawRect[0], this->backgroundDrawRect[1], this->backgroundDrawRect[2], this->backgroundDrawRect[3], this->imageHandle, TRUE);
	DrawStringToHandle(this->namePosition[0], this->namePosition[1], this->bossName.c_str(), this->TEXT_COLOR, this->fontHandle);
}

