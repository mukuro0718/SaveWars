#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameUI.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameUI::GameUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	const int GAME_CLEAR = asset.GetImage(LoadingAsset::ImageType::GAME_CLEAR);
	const int GAME_OVER = asset.GetImage(LoadingAsset::ImageType::GAME_OVER);
	this->image.emplace_back(new Image(GAME_CLEAR));
	this->image.emplace_back(new Image(GAME_OVER));
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameUI::~GameUI()
{
}

/// <summary>
/// ������
/// </summary>
void GameUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->alpha = Image::MAX_ALPHA;
		this->image[i]->isAddAlpha = false;
		this->image[i]->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_FIRST_DRAW_RECT"]);
	}
	this->isPrevPressAButton = false;
	this->isEndExtend = false;
	this->isEnd = false;
	this->type = -1;
	this->frameCount = 0;
}


/// <summary>
/// �X�V
/// </summary>
void GameUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	/*�^�C�v�̐ݒ�*/
	SetType();

	/*�܂��Q�[�����I�����Ă��Ȃ���Α������^�[��*/
	if (this->type == -1)return;

	/*�g�傪�I�����Ă��Ȃ���Ίg�債�đ������^�[��*/
	if (!this->isEndExtend)
	{
		this->isEndExtend = this->image[this->type]->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_TARGET_DRAW_RECT"], json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_ADD_VALUE"]);
		return;
	}

	/*�t���[���J�E���g���萔������������t���[���J�E���g�𑝉����đ������^�[��*/
	if (this->frameCount < json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"])
	{
		this->frameCount++;
		return;
	}

	/*�����ł͏k��������*/
	this->isEnd = this->image[this->type]->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_FIRST_DRAW_RECT"], json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DECREASE_VALUE"]);
	if (!isEnd)return;
}

/// <summary>
/// �`��
/// </summary>
const void GameUI::Draw()const
{
	/*�܂��Q�[�����I�����Ă��Ȃ���Α������^�[��*/
	if (this->type == -1)return;

	this->image[this->type]->Draw();
	
	if (this->type == static_cast<int>(Type::LOSE))
	{

	}
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