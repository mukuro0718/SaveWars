#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "UIManager.h"
#include "SceneState.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
UIManager::UIManager()
	:hp(nullptr)
{
	/*�C���X�^���X�̐���*/
	this->hp = new HPUI();
	this->button = new ButtonUI();

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->hp);
	DeleteMemberInstance(this->button);
}

/// <summary>
/// ������
/// </summary>
void UIManager::Initialize()
{
	this->hp->Initialize();
	this->button->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void UIManager::Update()
{
	auto& sceneState = Singleton<SceneState>::GetInstance();

	this->hp->Update();
	this->button->Update();
}

/// <summary>
/// �`��
/// </summary>
const void UIManager::Draw()const
{
	auto& sceneState = Singleton<SceneState>::GetInstance();

	this->hp->Draw();
	this->button->Draw();
}