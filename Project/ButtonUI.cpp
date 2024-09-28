#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ButtonUI::ButtonUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->healIcon = asset.GetImage(LoadingAsset::ImageType::HEAL_ICON);
	this->healOrb  = asset.GetImage(LoadingAsset::ImageType::HP_ORB);
	this->emptyOrb = asset.GetImage(LoadingAsset::ImageType::EMPTY_ORB);

	this->iconFont		= asset.GetFont(LoadingAsset::FontType::ICON_UI);
	this->operationFont = asset.GetFont(LoadingAsset::FontType::OPERATION_UI);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ButtonUI::~ButtonUI()
{

}

/// <summary>
/// ������
/// </summary>
void ButtonUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*������*/
}

/// <summary>
/// �X�V
/// </summary>
void ButtonUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�񕜃A�C�R���̃��L���X�g�^�C����\���ł���悤�ɂ�����*/
	
}

/// <summary>
/// �`��
/// </summary>
void ButtonUI::Draw()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�A�C�R���̕`��*/
	DrawIcon();

	/*�A�C�R�����Ƃ̕����̕`��*/
	DrawFont();
}


/// <summary>
/// �A�C�R���̕`��
/// </summary>
void ButtonUI::DrawIcon()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�ϐ��̏���*/
	Vec2d healIconPosition;
	Vec2d orbPosition;
	int orbXOffset = 0;
	int drawGraph = -1;

	/*json�f�[�^�̑��*/
	const int NOW_ORB_NUM = player.GetHealOrbNum();
	const int MAX_ORB_NUM = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];
	const int ORB_WIDTH   = json.GetJson(JsonManager::FileType::UI)["ORB_WIDTH"];
	orbPosition.Set(		json.GetJson(JsonManager::FileType::UI)["ORB_POSITION"]);
	healIconPosition.Set(	json.GetJson(JsonManager::FileType::UI)["HEAL_ICON_POSITION"]);


	/*�A�C�R���e�[�u���̕`��*/
	DrawGraph(healIconPosition.x, healIconPosition.y, this->healIcon, TRUE);

	/*�I�[�u�̕`��*/
	for (int i = 0; i < MAX_ORB_NUM; i++)
	{
		if (i - NOW_ORB_NUM < 0)
		{
			drawGraph = this->healOrb;
		}
		else
		{
			drawGraph = this->emptyOrb;
		}
		DrawGraph(orbPosition.x + orbXOffset, orbPosition.y, drawGraph, TRUE);
		orbXOffset += ORB_WIDTH;
	}
}

/// <summary>
/// �w�i�̕`��
/// </summary>
void ButtonUI::DrawFont()
{
	///*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	//auto& json = Singleton<JsonManager>::GetInstance();

	///*�ϐ��̏���*/
	//Vec2d mainAttackPosition,specialAttackPosition,avoidPosition,blockPosition,dashPosition;
	//int   textColor;

	///*json�f�[�^�̑��*/
	//mainAttackPosition		.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_TEXT_POSITION"]);
	//specialAttackPosition	.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_TEXT_POSITION"]);
	//avoidPosition			.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_TEXT_POSITION"]);
	//blockPosition			.Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_TEXT_POSITION"]);
	//dashPosition			.Set(json.GetJson(JsonManager::FileType::UI)["DASH_TEXT_POSITION"]);
	//textColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);


	///*�w�i�̕`��*/
	//DrawStringToHandle(mainAttackPosition.x	  , mainAttackPosition.y	, "�U��"	 , textColor, this->iconFont);
	//DrawStringToHandle(specialAttackPosition.x, specialAttackPosition.y	, "�X�L��"  , textColor, this->iconFont);
	//DrawStringToHandle(avoidPosition.x		  , avoidPosition.y			, "���"	 , textColor, this->iconFont);
	//DrawStringToHandle(blockPosition.x		  , blockPosition.y			, "�K�[�h"	 , textColor, this->iconFont);
	//DrawStringToHandle(dashPosition.x		  , dashPosition.y			, "�_�b�V��", textColor, this->iconFont);
}
/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}