#include <assert.h>
#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "SelectUI.h"
#include "GameUI.h"
#include "GameOverUI.h"
#include "GameClearUI.h"
#include "UIManager.h"
#include "SceneChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
UIManager::UIManager()
{
	this->scene.emplace_back(new TitleUI());
	this->scene.emplace_back(new SelectUI());
	this->scene.emplace_back(new GameUI());
}

/// <summary>
/// デストラクタ
/// </summary>
UIManager::~UIManager()
{
	for (int i = 0; i < this->scene.size(); i++)
	{
		DeleteMemberInstance(this->scene[i]);
	}
	this->scene.clear();
}

/// <summary>
/// 初期化
/// </summary>
void UIManager::Initialize()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void UIManager::Update()
{
	//int startTime = GetNowCount();
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->Update();
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// 描画
/// </summary>
const void UIManager::Draw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->Draw();
	//printfDx("UI_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// 描画しているか
/// </summary>
const bool UIManager::IsDraw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	return this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->IsEnd();
}
