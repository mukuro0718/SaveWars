#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "PlayerManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->table = asset.GetImage(LoadingAsset::ImageType::POTION_TABLE);
	this->potion = asset.GetImage(LoadingAsset::ImageType::POTION);
	this->healOrb  = asset.GetImage(LoadingAsset::ImageType::HP_ORB);
	this->emptyOrb = asset.GetImage(LoadingAsset::ImageType::EMPTY_ORB);

	this->iconFont		= asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);
	this->operationFont = asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);

	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::B_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::X_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::Y_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::LT_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_B_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_X_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_Y_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_LT_BUTTON));
	this->buttonFont = asset.GetFont(LoadingAsset::FontType::MINTYO_50_32);
}

/// <summary>
/// デストラクタ
/// </summary>
ButtonUI::~ButtonUI()
{

}

/// <summary>
/// 初期化
/// </summary>
void ButtonUI::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*初期化*/
}

/// <summary>
/// 更新
/// </summary>
void ButtonUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*回復アイコンのリキャストタイムを表示できるようにしたい*/
	
}

/// <summary>
/// 描画
/// </summary>
void ButtonUI::Draw()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*アイコンの描画*/
	DrawIcon();

	/*ボタンの描画*/
	DrawButton();

	/*アイコンごとの文字の描画*/
	DrawFont();
}


/// <summary>
/// アイコンの描画
/// </summary>
void ButtonUI::DrawIcon()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*変数の準備*/
	vector<int> potionPositon = json.GetJson(JsonManager::FileType::UI)["POTION_POSITION"];
	vector<int> tablePosition = json.GetJson(JsonManager::FileType::UI)["TABLE_POSITION"];
	vector<int> orbPosition = json.GetJson(JsonManager::FileType::UI)["ORB_POSITION"];
	//int orbXOffset = 0;
	int drawGraph = -1;

	/*jsonデータの代入*/
	const int NOW_ORB_NUM = player.GetHealOrbNum();
	//const int MAX_ORB_NUM = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];
	//const int ORB_WIDTH   = json.GetJson(JsonManager::FileType::UI)["ORB_WIDTH"];


	/*アイコンテーブルの描画*/
	DrawExtendGraph(tablePosition[0], tablePosition[1], tablePosition[2], tablePosition[3], this->table, TRUE);
	DrawExtendGraph(potionPositon[0], potionPositon[1], potionPositon[2], potionPositon[3], this->potion, TRUE);

	/*オーブの描画*/
	DrawFormatStringToHandle(orbPosition[0], orbPosition[1], this->TEXT_COLOR, this->buttonFont, "×%d", NOW_ORB_NUM);
}

void ButtonUI::DrawButton()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["BUTTON_POSITION"];
	int pad = input.GetPadState();
	vector<bool> isTrigger;
	isTrigger.emplace_back(pad & PAD_INPUT_4);
	isTrigger.emplace_back(pad & PAD_INPUT_1);
	isTrigger.emplace_back(pad & PAD_INPUT_2);
	isTrigger.emplace_back(pad & PAD_INPUT_7);

	int drawGraph = -1;
	for (int i = 0; i < position.size(); i++)
	{
		if (isTrigger[i])
		{
			drawGraph = this->button[i + this->PRESS_OFFSET];
		}
		else
		{
			drawGraph = this->button[i];
		}
		DrawExtendGraph(position[i][0], position[i][1], position[i][2], position[i][3], drawGraph, TRUE);
	}
}

/// <summary>
/// 背景の描画
/// </summary>
void ButtonUI::DrawFont()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["BUTTON_TEXT_POSITION"];
	vector<string> text;
	text.emplace_back(u8"かいひ");
	text.emplace_back(u8"こうげき");
	text.emplace_back(u8"かいふく");
	text.emplace_back(u8"ぼうぎょ");
	int pad = input.GetPadState();
	vector<bool> isTrigger;
	isTrigger.emplace_back(pad & PAD_INPUT_4);
	isTrigger.emplace_back(pad & PAD_INPUT_1);
	isTrigger.emplace_back(pad & PAD_INPUT_2);
	isTrigger.emplace_back(pad & PAD_INPUT_7);

	int textColor = 0;
	for (int i = 0; i < text.size(); i++)
	{
		if (isTrigger[i])
		{
			textColor = this->PRESS_TEXT_COLOR;
		}
		else
		{
			textColor = this->TEXT_COLOR;
		}
		switch (i)
		{
		case static_cast<int>(TextType::AVOID):
			DrawStringToHandle(position[i][0], position[i][1], "回避", textColor, this->buttonFont);
			break;
		case static_cast<int>(TextType::ATTACK):
			DrawStringToHandle(position[i][0], position[i][1], "攻撃", textColor, this->buttonFont);
			break;
		case static_cast<int>(TextType::HEAL):
			DrawStringToHandle(position[i][0], position[i][1], "回復", textColor, this->buttonFont);
			break;
		case static_cast<int>(TextType::GUARD):
			DrawStringToHandle(position[i][0], position[i][1], "防御", textColor, this->buttonFont);
			break;
		}
	}
}
/// <summary>
/// 色取得
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}