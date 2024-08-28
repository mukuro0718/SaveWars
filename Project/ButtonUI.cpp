#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "ButtonUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ButtonUI::ButtonUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->b = asset.GetImage(LoadingAsset::ImageType::B);
	this->x = asset.GetImage(LoadingAsset::ImageType::X);
	this->y = asset.GetImage(LoadingAsset::ImageType::Y);
	this->lb = asset.GetImage(LoadingAsset::ImageType::LB);
	this->ls = asset.GetImage(LoadingAsset::ImageType::LS);
	this->lt = asset.GetImage(LoadingAsset::ImageType::LT);
	this->rb = asset.GetImage(LoadingAsset::ImageType::RB);
	this->rs = asset.GetImage(LoadingAsset::ImageType::RS);
	this->iconFont = asset.GetFont(LoadingAsset::FontType::ICON_UI);
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
	//�T�C�Y
	this->specialAttackSize = 0;
}

/// <summary>
/// �X�V
/// </summary>
void ButtonUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*���ʂȍU���ɑ΂���t�B���^�[�̏������s���i�N�[���^�C���Q�[�W�j*/
	if (this->specialAttackSize <= json.GetJson(JsonManager::FileType::UI)["ICON_SIZE"])
	{
		this->specialAttackSize++;
	}
	
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

	/*�ϐ��̏���*/
	Box xBox,yBox, bBox, ltBox,lbBox,lsBox;
	/*json�f�[�^�̑��*/
	xBox.Set(json.GetJson(JsonManager::FileType::UI)["X_BOX"]);
	yBox.Set(json.GetJson(JsonManager::FileType::UI)["Y_BOX"]);
	bBox.Set(json.GetJson(JsonManager::FileType::UI)["B_BOX"]);
	ltBox.Set(json.GetJson(JsonManager::FileType::UI)["LT_BOX"]);
	lbBox.Set(json.GetJson(JsonManager::FileType::UI)["LB_BOX"]);
	lsBox.Set(json.GetJson(JsonManager::FileType::UI)["LS_BOX"]);

	/*�A�C�R���̕`��*/
	DrawExtendGraph(xBox.lx, xBox.ly, xBox.rx, xBox.ry, this->x, TRUE);
	DrawExtendGraph(yBox.lx, yBox.ly, yBox.rx, yBox.ry, this->y, TRUE);
	DrawExtendGraph(bBox.lx, bBox.ly, bBox.rx, bBox.ry, this->b, TRUE);
	DrawExtendGraph(ltBox.lx, ltBox.ly, ltBox.rx, ltBox.ry, this->lt, TRUE);
	DrawExtendGraph(lbBox.lx, lbBox.ly, lbBox.rx, lbBox.ry, this->lb, TRUE);
	DrawExtendGraph(lsBox.lx, lsBox.ly, lsBox.rx, lsBox.ry, this->ls, TRUE);
}

/// <summary>
/// �w�i�̕`��
/// </summary>
void ButtonUI::DrawFont()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ϐ��̏���*/
	Vec2d mainAttackPosition,specialAttackPosition,avoidPosition,blockPosition,dashPosition;
	int   textColor;

	/*json�f�[�^�̑��*/
	mainAttackPosition		.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_TEXT_POSITION"]);
	specialAttackPosition	.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_TEXT_POSITION"]);
	avoidPosition			.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_TEXT_POSITION"]);
	blockPosition			.Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_TEXT_POSITION"]);
	dashPosition			.Set(json.GetJson(JsonManager::FileType::UI)["DASH_TEXT_POSITION"]);
	textColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);


	/*�w�i�̕`��*/
	DrawStringToHandle(mainAttackPosition.x	  , mainAttackPosition.y	, "�U��"	 , textColor, this->iconFont);
	DrawStringToHandle(specialAttackPosition.x, specialAttackPosition.y	, "�X�L��"  , textColor, this->iconFont);
	DrawStringToHandle(avoidPosition.x		  , avoidPosition.y			, "���"	 , textColor, this->iconFont);
	DrawStringToHandle(blockPosition.x		  , blockPosition.y			, "�K�[�h"	 , textColor, this->iconFont);
	DrawStringToHandle(dashPosition.x		  , dashPosition.y			, "�_�b�V��", textColor, this->iconFont);
}
/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}