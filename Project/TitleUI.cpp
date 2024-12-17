#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleUI::TitleUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	const int TITLE_LOGO = asset.GetImage(LoadingAsset::ImageType::TITLE_LOGO);
	const int PRESS_BUTTON_LOGO = asset.GetImage(LoadingAsset::ImageType::PRESS_BUTTON_LOGO);
	this->image.emplace_back(new Image(TITLE_LOGO));
	this->image.emplace_back(new Image(PRESS_BUTTON_LOGO));
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
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->alpha = 0;
		this->image[i]->isAddAlpha = true;
	}
	this->image[static_cast<int>(Type::TITLE)]->SetPosition(json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_DRAW_RECT"]);
	this->image[static_cast<int>(Type::PRESS)]->SetPosition(json.GetJson(JsonManager::FileType::UI)["PRESS_A_DRAW_RECT"]);
	this->isPrevPressButton = false;
	this->isFadeOut = false;
}


/// <summary>
/// �X�V
/// </summary>
void TitleUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*pad����*/
	bool isPressButton = IsPressButton();
	//�O�Ƀ{�^����������Ă�����

	/*�A���t�@����*/
	int type = static_cast<int>(Type::TITLE);
	int addAlpha = json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ADD_ALPHA"];
	if (!this->isFadeOut)
	{
		//�^�C�g�����S�̃A���t�@���ő�A���t�@������������
		if (this->image[type]->alpha < Image::MAX_ALPHA)
		{
			//type���S�̃A���t�@�𑝉�������
			this->image[type]->FadeIn(addAlpha);
		}
		//�Ⴄ�Ȃ�type��PRESS
		else
		{
			type = static_cast<int>(Type::PRESS);
			addAlpha = json.GetJson(JsonManager::FileType::UI)["PRESS_A_ADD_ALPHA"];
			this->image[type]->FadeInOut(addAlpha);
		}
	}

	if (this->isFadeOut)
	{
		addAlpha = json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ADD_ALPHA"];
		this->image[static_cast<int>(Type::TITLE)]->FadeOut(addAlpha);
	}

	/*�{�^����������Ă�����A���t�@���ő�A���t�@�ɂ���*/
	if (isPressButton && !this->image[static_cast<int>(Type::TITLE)]->isAddAlpha)
	{
		this->isFadeOut = true;
		this->image[static_cast<int>(Type::PRESS)]->alpha = 0;
	}
}

/// <summary>
/// �`��
/// </summary>
const void TitleUI::Draw()const
{
	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->Draw();
	}
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool TitleUI::IsEnd()const
{
	/*PRESS�̃A���t�@���ϓ����Ă�����\�����Ă���*/
	if (this->isFadeOut && this->image[static_cast<int>(Type::TITLE)]->alpha <= 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �{�^���̓���
/// </summary>
bool TitleUI::IsPressButton()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad����*/
	int pad = input.GetNowPadState();
	bool isPressButton = false;

	/*PRESS���S���\������Ă�����*/
	if (this->image[static_cast<int>(Type::PRESS)]->alpha > 0)
	{
			isPressButton = (pad & PAD_INPUT_4);
			//�O�Ƀ{�^�����͂��Ȃ�&���{�^�����͂�����
			if (!this->isPrevPressButton && isPressButton)
			{
				this->isPrevPressButton = true;
			}
			//�O�Ƀ{�^�����͂�����
			else if (this->isPrevPressButton)
			{
				//���{�^�����͂��Ȃ�
				if (!isPressButton)
				{
					this->isPrevPressButton = false;
				}
				isPressButton = false;
			}
	}

	/*�����L�[�������ꂽ��A��������true��Ԃ�*/
	if (CheckHitKeyAll()) return true;


	return isPressButton;
}