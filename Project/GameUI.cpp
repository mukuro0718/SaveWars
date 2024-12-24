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
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_150_32);
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
	auto& json = Singleton<JsonManager>	 ::GetInstance();
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
	this->hp->Update();
	this->button->Update();
	this->bossName->Update();

	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	/*�^�C�v�̐ݒ�*/
	SetType();

	/*�܂��Q�[�����I�����Ă��Ȃ���Α������^�[��*/
	if (this->type == -1)return;

	/*�g�傪�I�����Ă��Ȃ���Ίg�債�đ������^�[��*/
	if (this->alpha < json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"])
	{
		this->alpha += json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
		return;
	}

	/*�t���[���J�E���g���萔�ȏゾ������I���t���O�𗧂Ă�*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"])
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

	/*�܂��Q�[�����I�����Ă��Ȃ���Α������^�[��*/
	if (this->type == -1)return;
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	vector<int> table = json.GetJson(JsonManager::FileType::UI)["GAME_TABLE_DRAW_RECT"];
	DrawExtendGraph(table[0], table[1], table[2], table[3], this->imageHandle, TRUE);
	vector<int> position1 = json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_1"];
	vector<int> position2 = json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_2"];
	if (this->type == static_cast<int>(Type::LOSE))
	{
		DrawStringToHandle(position1[0], position1[1], "��\n��", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(position2[0], position2[1], "��\n�s", this->TEXT_COLOR, this->fontHandle);
	}
	else
	{
		DrawStringToHandle(position1[0], position1[1], "��\n��", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(position2[0], position2[1], "��\n��", this->TEXT_COLOR, this->fontHandle);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"]);
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
	if (player.GetHP() < 0 && !player.GetIsAlive())
	{
		this->type = static_cast<int>(Type::LOSE);
	}
	/*�{�X��HP���O�ȉ�*/
	else if (enemy.GetHP() < 0 && !enemy.GetIsAlive())
	{
		this->type = static_cast<int>(Type::WIN);
	}
}