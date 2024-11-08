#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "PlayerManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ButtonUI::ButtonUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->table = asset.GetImage(LoadingAsset::ImageType::POTION_TABLE);
	this->potion = asset.GetImage(LoadingAsset::ImageType::POTION);

	this->iconFont		= asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);
	this->operationFont = asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);

	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::B_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::X_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::Y_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::A_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::RIGHT_TRIGGER));
	this->buttonFont = asset.GetFont(LoadingAsset::FontType::MINTYO_50_32);
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

	/*�{�^���̕`��*/
	DrawButton();

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
	vector<int> potionPositon = json.GetJson(JsonManager::FileType::UI)["POTION_POSITION"];
	vector<int> tablePosition = json.GetJson(JsonManager::FileType::UI)["TABLE_POSITION"];
	vector<int> orbPosition = json.GetJson(JsonManager::FileType::UI)["ORB_POSITION"];
	int drawGraph = -1;

	/*json�f�[�^�̑��*/
	const int NOW_ORB_NUM = player.GetHealOrbNum();

	/*�A�C�R���e�[�u���̕`��*/
	DrawExtendGraph(tablePosition[0], tablePosition[1], tablePosition[2], tablePosition[3], this->table, TRUE);
	DrawExtendGraph(potionPositon[0], potionPositon[1], potionPositon[2], potionPositon[3], this->potion, TRUE);

	/*�I�[�u�̕`��*/
	DrawFormatStringToHandle(orbPosition[0], orbPosition[1], this->TEXT_COLOR, this->buttonFont, "�~%d", NOW_ORB_NUM);
}

void ButtonUI::DrawButton()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["BUTTON_POSITION"];

	for (int i = 0; i < position.size(); i++)
	{
		DrawExtendGraph(position[i][0], position[i][1], position[i][2], position[i][3], this->button[i], TRUE);
	}
}

/// <summary>
/// �w�i�̕`��
/// </summary>
void ButtonUI::DrawFont()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["BUTTON_TEXT_POSITION"];
	const int TEXT_NUM = 5;

	int textColor = 0;
	for (int i = 0; i < TEXT_NUM; i++)
	{
		switch (i)
		{
		case static_cast<int>(TextType::AVOID):
			DrawStringToHandle(position[i][0], position[i][1], "���", this->TEXT_COLOR, this->buttonFont);
			break;
		case static_cast<int>(TextType::W_ATTACK):
			DrawStringToHandle(position[i][0], position[i][1], "��U��", this->TEXT_COLOR, this->buttonFont);
			break;
		case static_cast<int>(TextType::S_ATTACK):
			DrawStringToHandle(position[i][0], position[i][1], "���U��", this->TEXT_COLOR, this->buttonFont);
			break;
		case static_cast<int>(TextType::HEAL):
			DrawStringToHandle(position[i][0], position[i][1], "��", this->TEXT_COLOR, this->buttonFont);
			break;
		case static_cast<int>(TextType::GUARD):
			DrawStringToHandle(position[i][0], position[i][1], "�h��", this->TEXT_COLOR, this->buttonFont);
			break;
		}
	}
}
/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}