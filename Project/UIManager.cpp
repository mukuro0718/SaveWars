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
/// �R���X�g���N�^
/// </summary>
UIManager::UIManager()
{
	this->scene.emplace_back(new TitleUI());
	this->scene.emplace_back(new SelectUI());
	this->scene.emplace_back(new GameUI());
	this->scene.emplace_back(new GameClearUI());
	this->scene.emplace_back(new GameOverUI());
}

/// <summary>
/// �f�X�g���N�^
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
/// ������
/// </summary>
void UIManager::Initialize()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void UIManager::Update()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->Update();
}

/// <summary>
/// �`��
/// </summary>
const void UIManager::Draw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->Draw();
}

const bool UIManager::IsDraw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();
	return this->scene[static_cast<int>(sceneChanger.GetNextSceneType())]->IsEnd();
}

const bool UIManager::IsContinue()const
{
	int type = static_cast<int>(SceneChanger::SceneType::GAME_OVER);
	auto& scene = dynamic_cast<GameOverUI&>(*this->scene[type]);
	bool isContinue = scene.IsContinue();
	return isContinue;
}