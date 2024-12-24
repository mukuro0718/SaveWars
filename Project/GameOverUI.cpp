#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameOverUI.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameOverUI::GameOverUI()
	: isPrevPressButton	(false)
	, isContinue		(false)
	, isEnd				(false)
	, type				(-1)
	, imageHandle		(-1)
	, fontHandle		(-1)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);

	/*fontHandle�̎擾*/
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_80_64);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameOverUI::~GameOverUI()
{
}

/// <summary>
/// ������
/// </summary>
void GameOverUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	this->alpha				= 0;
	this->isContinue		= false;
	this->isPrevPressButton = false;
	this->type				= 0;
}


/// <summary>
/// �X�V
/// </summary>
void GameOverUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	bool isPressButton = IsPressButton();

	SetType();

	/*�g�傪�I�����Ă��Ȃ���Ίg�債�đ������^�[��*/
	if (this->alpha < json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"])
	{
		this->alpha += json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
		return;
	}

	/*�{�^����������Ă����烊�U���g�I��*/
	if (isPressButton)
	{
		if (this->type == static_cast<int>(ImageType::CONTINUE))
		{
			this->isContinue = true;
		}
		this->isEnd = true;
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameOverUI::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*�w�i�̕`��*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	vector<int> table = json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TABLE_POSITION"];
	DrawExtendGraph(table[0], table[1], table[2], table[3], this->imageHandle, TRUE);

	/*�����̕`��*/
	vector<int> position1 = json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TEXT_POSITION"][0];
	vector<int> position2 = json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TEXT_POSITION"][1];
	if (this->type == static_cast<int>(ImageType::CONTINUE))
	{
		DrawStringToHandle(position1[0], position1[1], "�Đ�", this->PRESS_TEXT_COLOR, this->fontHandle, TRUE);
		DrawStringToHandle(position2[0], position2[1], "�I��", this->TEXT_COLOR, this->fontHandle, TRUE);
	}
	else
	{
		DrawStringToHandle(position1[0], position1[1], "�Đ�", this->TEXT_COLOR, this->fontHandle, TRUE);
		DrawStringToHandle(position2[0], position2[1], "�I��", this->PRESS_TEXT_COLOR, this->fontHandle, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"]);
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool GameOverUI::IsEnd()const
{
	/*PRESS�̃A���t�@���ϓ����Ă�����\�����Ă���*/
	return this->isEnd;
}

/// <summary>
/// �Q�[���I�[�o�[���A�A�C�R���̎�ނ�ݒ�
/// </summary>
void GameOverUI::SetType()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	const int Y_BUF = input.GetLStickState().YBuf;

	/*��*/
	if (Y_BUF > 0)
	{
		this->type++;
		if (this->type >= static_cast<int>(ImageType::END))
		{
			this->type = static_cast<int>(ImageType::END);
		}
	}
	/*��*/
	else if (Y_BUF < 0)
	{
		this->type--;
		if (this->type <= static_cast<int>(ImageType::CONTINUE))
		{
			this->type = static_cast<int>(ImageType::CONTINUE);
		}
	}
}

/// <summary>
/// �{�^���̓���
/// </summary>
bool GameOverUI::IsPressButton()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad����*/
	int pad = input.GetNowPadState();
	bool isPressButton = false;

	/*window���ړ����Ă��Ȃ�/�g������Ă��Ȃ����*/
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

	return isPressButton;
}


const bool GameOverUI::IsContinue()const
{
	return this->isContinue;
}