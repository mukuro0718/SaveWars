#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameClearUI.h"
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
GameClearUI::GameClearUI()
	: window			(nullptr)
	, isPrevPressButton	(false)
	, isEnd				(false)
	, isEndExtend		(false)
	, isRedused			(false)
	, starIndex			(0)
	, frameCount		(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	this->window = new Image(asset.GetImage(LoadingAsset::ImageType::WINDOW));
	this->starFrame.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::NONE_STAR)));
	this->starFrame.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::NONE_STAR)));
	this->starFrame.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::NONE_STAR)));

	/*fontHandle�̎擾*/
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_100_64));
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_30_64));

	//Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameClearUI::~GameClearUI()
{
}

/// <summary>
/// ������
/// </summary>
void GameClearUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	SetStarHandle();

	this->window->alpha = Image::MAX_ALPHA;
	this->window->isAddAlpha = true;
	this->window->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_FIRST_POSITION"]);

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_STAR_POSITION"];
	for (int i = 0; i < this->star.size(); i++)
	{
		this->star[i]->alpha = Image::MAX_ALPHA;
		this->star[i]->isAddAlpha = true;
		this->star[i]->SetPosition(position);
		this->starFrame[i]->alpha = Image::MAX_ALPHA;
		this->starFrame[i]->isAddAlpha = true;
		this->starFrame[i]->SetPosition(position);
		position = AddPositionOffset(position, json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_STAR_OFFSET"]);
	}
	this->isPrevPressButton = false;
	this->isEnd = false;
	this->isEndExtend = false;
	this->isRedused = false;
	this->starIndex = false;
	this->frameCount = 0;
}

/// <summary>
/// �X�V
/// </summary>
void GameClearUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*�{�^������*/
	bool isPressButton = IsPressButton();

	/*�e�L�X�g�C���f�b�N�X�̒ǉ�*/
	AddStarIndex();

	/*window�̊g�傪�I����Ă��邩*/
	if (!this->isEndExtend)
	{
		this->isEndExtend = this->window->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_TARGET_POSITION"], json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_ADD_VALUE"]);
		return;
	}


	if (isPressButton && this->starIndex == this->star.size())
	{
		this->isRedused = true;
	}

	if (this->isRedused)
	{
		this->isEnd = this->window->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_FIRST_POSITION"], json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_ADD_VALUE"]);
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameClearUI::Draw()const
{
	/*window�̕`��*/
	this->window->Draw();

	/*window�̊g�傪�I�����Ă��Ȃ���Α������^�[��*/
	if (!this->isEndExtend)return;

	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*���̕`��*/
	for (int i = 0; i < this->starFrame.size(); i++)
	{
		this->starFrame[i]->Draw();
	}
	for (int i = 0; i < this->starIndex; i++)
	{
		this->star[i]->Draw();
	}


	/*�e�L�X�g�̕`��*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_HEADER_POSITION"];
		string text = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_HEADER"];
		DrawStringToHandle(position[0], position[1], text.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::HEADER)]);
	}
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_MAIN_POSITION"];
		string text = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_MAIN"];
		for (int i = 0; i < text.size(); i++)
		{
			DrawStringToHandle(position[0], position[1], text.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::MAIN)]);
		}
	}
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool GameClearUI::IsEnd()const
{
	/*PRESS�̃A���t�@���ϓ����Ă�����\�����Ă���*/
	return this->isEnd;
}



/// <summary>
/// �{�^���̓���
/// </summary>
bool GameClearUI::IsPressButton()
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

/// <summary>
/// �e�L�X�g�C���f�b�N�X�̒ǉ�
/// </summary>
void GameClearUI::AddStarIndex()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*�g�傪�I�����Ă��Ȃ���Α������^�[��*/
	if (!this->isEndExtend)return;
	/*�C���f�b�N�X���萔�𒴂��Ă����瑁�����^�[��*/
	if (this->starIndex >= json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_STAR_NUM"])return;

	this->frameCount++;
	/*�t���[���J�E���g���萔�𒴂��Ă�����*/
	if (this->frameCount >= json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_ADD_STAR_INDEX_INTERVAL"])
	{
		this->frameCount = 0;
		this->starIndex++;
	}
}


void GameClearUI::SetStarHandle()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	int goldStar = asset.GetImage(LoadingAsset::ImageType::GOLD_STAR);
	int silverStar = asset.GetImage(LoadingAsset::ImageType::SILVER_STAR);
	int time = 0;//�o�ߎ���
	int orbNum = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"] - player.GetHealOrbNum();//�I�[�u�̎c��
	int continueCount = -1;
	vector<int> judge = { time,orbNum,continueCount };
	vector<int> goldLine = json.GetJson(JsonManager::FileType::UI)["GOLD_LINE"];
	vector<int> silverLine = json.GetJson(JsonManager::FileType::UI)["SILVER_LINE"];


	this->star.clear();
	for (int i = 0; i < goldLine.size(); i++)
	{
		/*���Ԃ��V���o�[���C���𒴂��Ă�����*/
		if (judge[i] <= silverLine[i])
		{
			//�S�[���h���C��
			if (judge[i] <= goldLine[i])
			{
				this->star.emplace_back(new Image(goldStar));
			}
			else
			{
				this->star.emplace_back(new Image(silverStar));
			}
		}
		else
		{
			this->star.emplace_back(new Image(-1));
		}
	}
}


vector<int> GameClearUI::AddPositionOffset(const vector<int> _position,const vector<int> _offset)
{
	vector<int> out = _position;
	for (int i = 0; i < out.size(); i++)
	{
		out[i] += _offset[i];
	}
	return out;
}