#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ButtonUI::ButtonUI()
{
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
	/*�w�i�̕`��*/
	DrawBackground();

	/*�A�C�R���̕`��*/
	DrawIcon();

	/*�A�C�R�����Ƃ̕����̕`��*/
	DrawIconText();

	/*�{�^������*/
	DrawButtonText();

	/*�������UI�̕`��*/
	DrawOperation();
}

void ButtonUI::DrawOperation()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ϐ��̏���*/
	Box   dashIcon1, dashIcon2, dashIcon3;
	Vec2d dashText;
	Vec2d dashButton1, dashButton2, dashButton3;
	int iconColor, buttonColor, textColor,fontSize;

	/*json�̑��*/
	dashIcon1	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_ICON_1_POSITION"]);
	dashIcon3	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_ICON_3_POSITION"]);
	dashText	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_ICON_TEXT_POSITION"]);
	dashButton1	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_BUTTON_1_TEXT_POSITION"]);
	dashButton2	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_BUTTON_2_TEXT_POSITION"]);
	dashButton3	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_BUTTON_3_TEXT_POSITION"]);
	iconColor   = ConvertColor(json.GetJson(JsonManager::FileType::UI)["GLAY_COLOR"]);
	buttonColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["LB_TEXTCOLOR"]);
	textColor	= ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);
	fontSize = json.GetJson(JsonManager::FileType::UI)["OPERATION_FONT_SIZE"];



	/*�`��*/
	SetFontSize(fontSize);
	DrawBox(dashIcon1.lx, dashIcon1.ly, dashIcon1.rx, dashIcon1.ry, iconColor, TRUE);
	DrawBox(dashIcon3.lx, dashIcon3.ly, dashIcon3.rx, dashIcon3.ry, iconColor, TRUE);
	DrawString(dashText.x, dashText.y, "�_�b�V��", textColor);
	DrawString(dashButton1.x, dashButton1.y, "L", buttonColor);
	DrawString(dashButton2.x, dashButton2.y, "+", buttonColor);
	DrawString(dashButton3.x, dashButton3.y, "LB", buttonColor);

}

/// <summary>
/// �{�^�������̕`��
/// </summary>
void ButtonUI::DrawButtonText()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ϐ��̏���*/
	Vec2d mainTextPosition;
	Vec2d specialTextPosition;
	Vec2d avoidTextPosition;
	Vec2d blockTextPosition;
	int   mainAttackColor;
	int   specialAttackColor;
	int   avoidColor;
	int   blockColor;
	int   fontSize;

	/*json�f�[�^�̑��*/
	mainTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["X_TEXT_POSITION"]);
	specialTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["Y_TEXT_POSITION"]);
	avoidTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["B_TEXT_POSITION"]);
	blockTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["LB_TEXT_POSITION"]);
	mainAttackColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["X_TEXT_COLOR"]);
	specialAttackColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["Y_TEXT_COLOR"]);
	avoidColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["B_TEXT_COLOR"]);
	blockColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["LB_TEXTCOLOR"]);
	fontSize = json.GetJson(JsonManager::FileType::UI)["BUTTON_FONT_SIZE"];

	/*�e�L�X�g�̕`��*/
	SetFontSize(fontSize);
	DrawString(mainTextPosition.x, mainTextPosition.y, "X", mainAttackColor);
	DrawString(specialTextPosition.x, specialTextPosition.y, "Y", specialAttackColor);
	DrawString(avoidTextPosition.x, avoidTextPosition.y, "B", avoidColor);
	DrawString(blockTextPosition.x, blockTextPosition.y, "LB", blockColor);
}

/// <summary>
/// �����̕`��
/// </summary>
void ButtonUI::DrawIconText()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ϐ��̏���*/
	Vec2d mainTextPosition;
	Vec2d specialTextPosition;
	Vec2d avoidTextPosition;
	Vec2d blockTextPosition;
	int   fontSize;
	int   textEdgeColor;
	int   textColor;

	/*json�f�[�^�̑��*/
	mainTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_TEXT_POSITION"]);
	specialTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_TEXT_POSITION"]);
	avoidTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_TEXT_POSITION"]);
	blockTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_TEXT_POSITION"]);
	fontSize = json.GetJson(JsonManager::FileType::UI)["ICON_FONT_SIZE"];
	textEdgeColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_EDGE_COLOR"]);
	textColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);

	/*�e�L�X�g�̕`��*/
	SetFontSize(fontSize);
	DrawString(mainTextPosition.x, mainTextPosition.y, "��{�U��", textColor, textEdgeColor);
	DrawString(specialTextPosition.x, specialTextPosition.y, "����U��", textColor, textEdgeColor);
	DrawString(avoidTextPosition.x, avoidTextPosition.y, "���", textColor, textEdgeColor);
	DrawString(blockTextPosition.x, blockTextPosition.y, "�K�[�h", textColor, textEdgeColor);
}

/// <summary>
/// �A�C�R���̕`��
/// </summary>
void ButtonUI::DrawIcon()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ϐ��̏���*/
	Vec2d mainAttackPosition;
	Vec2d specialAttackPosition;
	Vec2d avoidPosition;
	Box   blockPosition;
	int   iconSize;
	int   mainAttackColor;
	int   specialAttackColor;
	int   avoidColor;
	int   blockColor;

	/*json�f�[�^�̑��*/
	mainAttackPosition	 .Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_POSITION"]);
	specialAttackPosition.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_POSITION"]);
	avoidPosition		 .Set(json.GetJson(JsonManager::FileType::UI)["AVOID_POSITION"]);
	blockPosition		 .Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_POSITION"]);
	iconSize			 = json.GetJson(JsonManager::FileType::UI)["ICON_SIZE"];
	mainAttackColor		 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BLUE_COLOR"]);
	specialAttackColor	 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["YELLOW_COLOR"]);
	avoidColor			 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["RED_COLOR"]);
	blockColor			 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["GLAY_COLOR"]);

	/*�A�C�R���̕`��*/
	DrawCircle(mainAttackPosition.x	  , mainAttackPosition.y	, iconSize				 , mainAttackColor	 , TRUE);
	DrawCircle(specialAttackPosition.x, specialAttackPosition.y	, this->specialAttackSize, specialAttackColor, TRUE);
	DrawCircle(avoidPosition.x		  , avoidPosition.y			, iconSize				 , avoidColor		 , TRUE);
	DrawBox(blockPosition.lx, blockPosition.ly, blockPosition.rx, blockPosition.ry, blockColor, TRUE);
}

/// <summary>
/// �w�i�̕`��
/// </summary>
void ButtonUI::DrawBackground()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ϐ��̏���*/
	Vec2d mainAttackPosition;
	Vec2d specialAttackPosition;
	Vec2d avoidPosition;
	int   iconSize;
	int   thickness;
	int   backgroundColor;

	/*json�f�[�^�̑��*/
	mainAttackPosition		.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_POSITION"]);
	specialAttackPosition	.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_POSITION"]);
	avoidPosition			.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_POSITION"]);
	iconSize				= json.GetJson(JsonManager::FileType::UI)["ICON_SIZE"];
	thickness				= json.GetJson(JsonManager::FileType::UI)["BACKGROUND_THICKNESS"];
	backgroundColor			= ConvertColor(json.GetJson(JsonManager::FileType::UI)["BACKGROUND_COLOR"]);


	/*�w�i�̕`��*/
	DrawCircle(mainAttackPosition.x, mainAttackPosition.y, iconSize, backgroundColor, TRUE, thickness);
	DrawCircle(specialAttackPosition.x, specialAttackPosition.y, iconSize, backgroundColor, TRUE, thickness);
	DrawCircle(avoidPosition.x, avoidPosition.y, iconSize, backgroundColor, TRUE, thickness);
}
/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}