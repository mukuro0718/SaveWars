#include <assert.h>
#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "TutorialUI.h"
#include "GameUI.h"
#include "GameOverUI.h"
#include "GameClearUI.h"
#include "UIManager.h"
#include "SceneChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
UIManager::UIManager()
	:hp(nullptr)
{
	/*�C���X�^���X�̐���*/
	this->hp = new HPUI();
	this->button = new ButtonUI();
	this->scene.emplace_back(new TitleUI());
	this->scene.emplace_back(new TutorialUI());
	this->scene.emplace_back(new GameUI());
	this->scene.emplace_back(new GameClearUI());
	this->scene.emplace_back(new GameOverUI());
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->hp);
	DeleteMemberInstance(this->button);
	for (int i = 0; i < this->scene.size(); i++)
	{
		DeleteMemberInstance(this->scene[i]);
	}
}

/// <summary>
/// ������
/// </summary>
void UIManager::Initialize()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNextSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->Initialize();
		break;
	case SceneChanger::SceneType::TUTORIAL:
		this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->Initialize();
		break;
	case SceneChanger::SceneType::GAME:
		this->hp->Initialize();
		this->button->Initialize();
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->Initialize();
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->Initialize();
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->Initialize();
		break;
	}
}

/// <summary>
/// �X�V
/// </summary>
void UIManager::Update()
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->Update();
		break;
	case SceneChanger::SceneType::TUTORIAL:
		this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->Update();
		break;
	case SceneChanger::SceneType::GAME:
		this->hp->Update();
		this->button->Update();
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->Update();
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->Update();
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->Update();
		break;
	}
}

/// <summary>
/// �`��
/// </summary>
const void UIManager::Draw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->Draw();
		break;
	case SceneChanger::SceneType::TUTORIAL:
		this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->Draw();
		break;
	case SceneChanger::SceneType::GAME:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->Draw();
		this->hp->Draw();
		this->button->Draw();
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->Draw();
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->Draw();
		break;
	}
}

const bool UIManager::IsDraw()const
{
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		return this->scene[static_cast<int>(SceneChanger::SceneType::TITLE)]->IsEnd();
	case SceneChanger::SceneType::TUTORIAL:
		return this->scene[static_cast<int>(SceneChanger::SceneType::TUTORIAL)]->IsEnd();
	case SceneChanger::SceneType::GAME:
		return this->scene[static_cast<int>(SceneChanger::SceneType::GAME)]->IsEnd();
	case SceneChanger::SceneType::GAME_CLEAR:
		return this->scene[static_cast<int>(SceneChanger::SceneType::GAME_CLEAR)]->IsEnd();
	case SceneChanger::SceneType::GAME_OVER:
		return this->scene[static_cast<int>(SceneChanger::SceneType::GAME_OVER)]->IsEnd();
	}
	return false;
}

const bool UIManager::IsContinue()const
{
	int type = static_cast<int>(SceneChanger::SceneType::GAME_OVER);
	auto* scene = dynamic_cast<GameOverUI*>(this->scene[type]);

	assert(scene == nullptr && "scene pointer is null");

	return scene->IsContinue();
}