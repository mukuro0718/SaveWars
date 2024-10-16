#include <DxLib.h>
#include<iostream>
#include<fstream>
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
#include "Timer.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameClearUI::GameClearUI()
	: imageHandle	(-1)
	, fontHandle	(-1)
	, alpha			(0)
	, nowRanking	(-1)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);

	/*fontHandle�̎擾*/
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);

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


	this->isEnd				= false;
	this->isPrevPressButton = false;
	this->alpha				= 0;
	this->nowRanking		= -1;

	/*�����L���O�̍X�V*/
	CalcRanking();
}

/// <summary>
/// �X�V
/// </summary>
void GameClearUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*�g�傪�I�����Ă��Ȃ���Ίg�債�đ������^�[��*/
	if (this->alpha < json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"])
	{
		this->alpha += json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
		return;
	}

	/*�{�^������*/
	bool isPressButton = IsPressButton();
	
	if (isPressButton)
	{
		this->isEnd = true;
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameClearUI::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& timer = Singleton<Timer>	::GetInstance();

	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	vector<int> table = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TABLE_POSITION"];
	DrawExtendGraph(table[0], table[1], table[2], table[3], this->imageHandle, TRUE);

	/*�N���A�^�C���̕\��*/
	vector<int> clearTimePosition = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TIME_POSITION"];
	int second = timer.GetNowTime() / 60;
	int minute = second / 60;
	second -= minute * 60;
	DrawFormatStringToHandle(clearTimePosition[0], clearTimePosition[1], this->TEXT_COLOR, this->fontHandle, "�������� %d��%d�b", minute, second);

	/*���ʂ̕\��*/
	vector<int> rankingPosition = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_RANKING_POSITION"];
	if (this->nowRanking == -1)
	{
		DrawStringToHandle(rankingPosition[0], rankingPosition[1],"�������� �s��" ,this->TEXT_COLOR, this->fontHandle);
	}
	else
	{
		DrawFormatStringToHandle(rankingPosition[0], rankingPosition[1], this->TEXT_COLOR, this->fontHandle, "�������� %d", this->nowRanking + 1);
	}

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_BACK_TITLE_POSITION"];
	DrawStringToHandle(position[0], position[1], "B�Ń^�C�g���ɖ߂�", this->TEXT_COLOR, this->fontHandle);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"]);

}

void GameClearUI::CalcRanking()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& timer = Singleton<Timer>	::GetInstance();

	int clearTime = timer.GetNowTime();//�����ɃN���A�^�C��������


	//�t�@�C����ǂݍ���œ��e����ʂɕ\������
	string fileName = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_RANKING_FILE_NAME"];

	ifstream ifs(fileName.c_str());
	if (ifs.good())
	{
		nlohmann::json nowJson;
		ifs >> nowJson;

		//�ǂݍ��񂾃f�[�^�����ꂼ��̕ϐ��ɑ������
		vector<int>	ranking = nowJson["RANKING"];

		int index = ranking.size();
		//�N���A�^�C���̊m�F
		for (int i = 0; i < index; i++)
		{
			//�����N�C������Ȃ�A���ʂ�ێ����ARanking���X�V����
			if (ranking[i] >= clearTime)
			{
				this->nowRanking = i;
				ranking.insert(ranking.begin() + i, clearTime);
				break;
			}
		}
		//���������N�C�����Ă��Ȃ��Ƃ��ɁA�C���f�b�N�X���ő喢����������ǉ�����
		if (this->nowRanking == -1 && index != json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_MAX_RANKING_USER"])
		{
			ranking.emplace_back(clearTime);
			this->nowRanking = index;
		}
		//�t�@�C���ɏ������ޓ��e
		nowJson =
		{
			{"RANKING",ranking },
		};
		//�쐬�����t�@�C���ɓ��e����������
		ofstream writing_file;
		writing_file.open(fileName, ios::out);
		writing_file << nowJson.dump() << endl;
		writing_file.close();

	}
	//�t�@�C�����Ȃ��ꍇ�͐V�������
	else
	{
		string indexName = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_RANKING_INDEX_NAME"];
		vector<int> value;
		value.emplace_back(clearTime);
		value.emplace_back(10000);
		nlohmann::json newJson = 
		{ 
			{indexName.c_str(),value},
		};
		//�쐬�����t�@�C���ɓ��e����������
		ofstream writing_file;
		writing_file.open(fileName, ios::out);
		writing_file << newJson.dump() << endl;
		writing_file.close();
		this->nowRanking = 1;
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

