#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"

//LoadingAsset* Singleton<LoadingAsset>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
LoadingAsset::LoadingAsset()
{
	/*json読み込みクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json内のパスを取得する*/
	vector<string> modelPath = json.GetJson(JsonManager::FileType::MODEL_PATH)["PATH"];
	vector<string> imagePath = json.GetJson(JsonManager::FileType::IMAGE_PATH)["PATH"];
	vector<string> fontPath  = json.GetJson(JsonManager::FileType::FONT_PATH )["PATH"];

	/*モデルのロード*/
	for (int i = 0; i < modelPath.size(); i++)
	{
		this->modelHandle.emplace_back(MV1LoadModel(modelPath[i].c_str()));
	}

	/*画像のロード*/
	for (int i = 0; i < imagePath.size(); i++)
	{
		this->imageHandle.emplace_back(LoadGraph(imagePath[i].c_str()));
	}

	/*フォントのロード*/
	for (int i = 0; i < fontPath.size(); i++)
	{
		this->fontHandle.emplace_back(CreateFontToHandle(fontPath[i].c_str(), 32, 16));
	}
}

/// <summary>
/// デストラクタ
/// </summary>
LoadingAsset::~LoadingAsset()
{
	/*アセットハンドルの解放*/
	for (int i = 0; i < this->modelHandle.size(); i++)
	{
		MV1DeleteModel(this->modelHandle[i]);
	}
	for (int i = 0; i < this->imageHandle.size(); i++)
	{
		DeleteGraph(this->imageHandle[i]);
	}
	for (int i = 0; i < this->soundHandle.size(); i++)
	{
		DeleteSoundMem(this->soundHandle[i]);
	}
	for (int i = 0; i < fontHandle.size(); i++)
	{
		DeleteFontToHandle(this->fontHandle[i]);
	}

	/*動的配列の開放*/
	this->fontHandle.clear();
	this->imageHandle.clear();
	this->modelHandle.clear();
	this->soundHandle.clear();
}
