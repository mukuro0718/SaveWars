#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameOverUI.h"
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
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	this->image.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::CONTINUE)));
	this->image.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::END)));

	Initialize();
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

	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->alpha = Image::MAX_ALPHA;
		this->image[i]->isAddAlpha = true;
		this->image[i]->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_POSITION"][i]);
	}
	this->isEndExtend = false;
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

	/*���̑傫���܂ŉ摜���g��*/
	if (!this->isEndExtend)
	{
		int endCount = 0;
		int index = this->image.size();
		for (int i = 0; i < index; i++)
		{
			bool isEnd = this->image[i]->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TARGET_POSITION"][i], json.GetJson(JsonManager::FileType::UI)["GAME_OVER_ADD_VALUE"]);
			if (isEnd)
			{
				endCount++;
			}
		}
		if (endCount == index)
		{
			this->isEndExtend = true;
		}
	}
	else
	{
		/*�g�債�I�������A�I�𒆂̂��̂��g��k��������*/
		for (int i = 0; i < this->image.size(); i++)
		{
			if (this->type == i)
			{
				this->image[i]->ScalingGraph(json.GetJson(JsonManager::FileType::UI)["RESULT_IMAGE_TARGET_DRAW_RECT"][i], json.GetJson(JsonManager::FileType::UI)["GAME_OVER_REDUCED_POSITION"][i], json.GetJson(JsonManager::FileType::UI)["GAME_OVER_ADD_VALUE"]);
			}
			else
			{
				this->image[i]->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TARGET_POSITION"][i]);
			}
		}
		/*�{�^����������Ă����烊�U���g�I��*/
		if (isPressButton)
		{
			//type��Continue��������true/End��������false��Ԃ�
			this->isEnd = true;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameOverUI::Draw()const
{
	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->Draw();
	}
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

	const int X_BUF = input.GetLStickState().XBuf;

	/*�E*/
	if (X_BUF > 0)
	{
		this->type++;
		if (this->type >= static_cast<int>(ImageType::END))
		{
			this->type = static_cast<int>(ImageType::END);
		}
	}
	/*��*/
	else if (X_BUF < 0)
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
	int pad = input.GetPadState();
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