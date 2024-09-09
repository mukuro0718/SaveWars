#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleUI::TitleUI()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleUI::~TitleUI()
{

}

/// <summary>
/// ������
/// </summary>
void TitleUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();


	this->logo.SetPosition(json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_DRAW_RECT"]);
	this->logo.alpha = 0;
	this->logo.imageHandle = asset.GetImage(LoadingAsset::ImageType::TITLE_LOGO);
	this->logo.isAddAlpha = true;

	this->pressA.SetPosition(json.GetJson(JsonManager::FileType::UI)["PRESS_A_DRAW_RECT"]);
	this->pressA.alpha = 0;
	this->pressA.imageHandle = asset.GetImage(LoadingAsset::ImageType::PRESS_A);
	this->pressA.isAddAlpha = true;
}

/// <summary>
/// �X�V
/// </summary>
void TitleUI::Update() 
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�A���t�@�̍X�V*/
	this->logo.AddAlpha(json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ADD_ALPHA"]);
	this->pressA.FlashingAlpha(json.GetJson(JsonManager::FileType::UI)["PRESS_A_ADD_ALPHA"]);
} 

/// <summary>
/// �`��
/// </summary>
const void TitleUI::Draw() const
{
	this->logo.Draw();
	this->pressA.Draw();
}