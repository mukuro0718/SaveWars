#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "HP.h"
#include "UIManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
UIManager::UIManager()
	:hp(nullptr)
{
	/*�C���X�^���X�̐���*/
	this->hp = new HP();

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIManager::~UIManager()
{
	DeleteMemberInstance(this->hp);
}

/// <summary>
/// ������
/// </summary>
void UIManager::Initialize()
{
	this->hp->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void UIManager::Update()
{
	this->hp->Update();
}

/// <summary>
/// �`��
/// </summary>
const void UIManager::Draw()const
{
	this->hp->Draw();
}