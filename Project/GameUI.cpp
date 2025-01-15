#include <DxLib.h>
#include "DeleteInstance.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "BossNameUI.h"
#include "SceneUI.h"
#include "GameUI.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameUI::GameUI()
	: hp(nullptr)
	, button(nullptr)
	, bossName(nullptr)
{
	/*�C���X�^���X�̐���*/
	this->hp = new HPUI();
	this->button = new ButtonUI();
	this->bossName = new BossNameUI();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->imageHandle		= asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);
	this->fontHandle		= asset.GetFont(LoadingAsset::FontType::MINTYO_150_32);
	this->pauseFontHandle	= asset.GetFont(LoadingAsset::FontType::MINTYO_50_32);

	auto& json = Singleton<JsonManager>	 ::GetInstance();
	this->maxAlpha					= json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"];
	this->addAlpha					= json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
	this->logoDrawTime				= json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"];
	vector<int> tableDrawRect		= json.GetJson(JsonManager::FileType::UI)["GAME_TABLE_DRAW_RECT"];
	this->tableDrawRect				= tableDrawRect;
	vector<int> destroyTextPosition = json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_1"];
	this->destroyTextPosition		= destroyTextPosition;
	vector<int> resultTextPosition	= json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_2"];
	this->resultTextPosition		= resultTextPosition;
	vector<int> pauseTableDrawRect = json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_TABLE_DRAW_RECT"];
	this->pauseTableDrawRect = pauseTableDrawRect;
	vector<int> pauseTextPosition = json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_TEXT_POSITION"];
	this->pauseTextPosition = pauseTextPosition;
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameUI::~GameUI()
{
	DeleteMemberInstance(this->hp);
	DeleteMemberInstance(this->button);
	DeleteMemberInstance(this->bossName);
}

/// <summary>
/// ������
/// </summary>
void GameUI::Initialize()
{
	/*�N���X�̏�����*/
	this->hp->Initialize();
	this->button->Initialize();
	this->bossName->Initialize();

	/*�ϐ��̏�����*/
	this->alpha = 0;
	this->isEnd = false;
	this->type = -1;
	this->frameCount = 0;
}


/// <summary>
/// �X�V
/// </summary>
void GameUI::Update()
{
	/*���fUI�����邩�ǂ����̔���*/
	auto& input = Singleton<InputManager>::GetInstance();
	//���f�t���O�������Ă��Ȃ����
	if (!this->isPause)
	{
		//PAD��START�������Ă�����
		if (input.GetNowPadState() & InputManager::PAD_START)
		{
			this->isPause = true;
		}
	}
	//�����Ă�����
	else
	{
		//B��������Ă�����
		if (input.GetNowPadState() & InputManager::PAD_B)
		{
			this->type = static_cast<int>(Type::LOSE);
			this->isPause = false;
		}
		//A��������Ă�����
		else if (input.GetNowPadState() & InputManager::PAD_A)
		{
			this->isPause = false;
		}
	}

	this->hp->Update();
	this->button->Update();
	this->bossName->Update();

	/*�^�C�v�̐ݒ�*/
	SetType();

	/*�܂��Q�[�����I�����Ă��Ȃ���Α������^�[��*/
	if (this->type == -1)return;

	/*�g�傪�I�����Ă��Ȃ���Ίg�債�đ������^�[��*/
	if (this->alpha < this->maxAlpha)
	{
		this->alpha += this->addAlpha;
		return;
	}

	/*�t���[���J�E���g���萔�ȏゾ������I���t���O�𗧂Ă�*/
	this->frameCount++;
	if (this->frameCount >= this->logoDrawTime)
	{
		this->isEnd = true;
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameUI::Draw()const
{
	this->hp->Draw();
	this->button->Draw();
	this->bossName->Draw();

	/*���f�t���O�������Ă�����UI��\������*/
	if (this->isPause)
	{
		DrawExtendGraph(this->pauseTableDrawRect[0], this->pauseTableDrawRect[1], this->pauseTableDrawRect[2], this->pauseTableDrawRect[3], this->imageHandle, TRUE);
		DrawStringToHandle(this->pauseTextPosition[0], this->pauseTextPosition[1], "�Q�[���𒆒f���܂���\nB:�͂� A:������", this->TEXT_COLOR, this->pauseFontHandle);

	}

	/*�܂��Q�[�����I�����Ă��Ȃ���Α������^�[��*/
	if (this->type == -1)return;
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->tableDrawRect[0], this->tableDrawRect[1], this->tableDrawRect[2], this->tableDrawRect[3], this->imageHandle, TRUE);
	if (this->type == static_cast<int>(Type::LOSE))
	{
		DrawStringToHandle(this->destroyTextPosition[0], this->destroyTextPosition[1], "��\n��", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(this->resultTextPosition[0], this->resultTextPosition[1], "��\n�s", this->TEXT_COLOR, this->fontHandle);
	}
	else
	{
		DrawStringToHandle(this->destroyTextPosition[0], this->destroyTextPosition[1], "��\n��", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(this->resultTextPosition[0], this->resultTextPosition[1], "��\n��", this->TEXT_COLOR, this->fontHandle);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->maxAlpha);
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool GameUI::IsEnd()const
{
	return this->isEnd;
}

/// <summary>
/// ��ނ̐ݒ�
/// </summary>
void GameUI::SetType()
{
	/*-1����Ȃ���Α������^�[��*/
	if (this->type != -1)return;
	
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*�v���C���[��HP���O�ȉ�*/
	if (player.GetHP() <= 0 && !player.GetIsAlive())
	{
		this->type = static_cast<int>(Type::LOSE);
	}
	/*�{�X��HP���O�ȉ�*/
	else if (enemy.GetHP() <= 0 && !enemy.GetIsAlive())
	{
		this->type = static_cast<int>(Type::WIN);
	}
}