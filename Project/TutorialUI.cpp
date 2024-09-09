#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneUI.h"
#include "TutorialUI.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TutorialUI::TutorialUI()
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TutorialUI::~TutorialUI()
{

}

/// <summary>
/// ������
/// </summary>
void TutorialUI::Initialize()
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
void TutorialUI::Update()
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
const void TutorialUI::Draw() const
{
	this->logo.Draw();
	this->pressA.Draw();
}