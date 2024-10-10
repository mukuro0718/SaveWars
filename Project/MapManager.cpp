#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "BossMap.h"
#include "Skydome.h"
#include "MapManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
MapManager::MapManager()
	: ground(nullptr)
	, skydome(nullptr)
{
	this->ground  = new BossMap();
	this->skydome = new Skydome();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapManager::~MapManager()
{
	DeleteMemberInstance(this->ground);
	DeleteMemberInstance(this->skydome);
}

/// <summary>
/// ������
/// </summary>
void MapManager::Initialize()
{
	this->ground->Initialize();
	this->skydome->Initialize();
}

/// <summary>
/// �㏈��
/// </summary>
void MapManager::Finalize()
{
	this->ground->Finalize();
}

/// <summary>
/// �X�V
/// </summary>
void MapManager::Update()
{
	this->ground->Update();
	this->skydome->Update();
}

/// <summary>
/// �`��
/// </summary>
const void MapManager::Draw()const
{
	this->ground->Draw();
	this->skydome->Draw();
}


const int MapManager::GetStageModelHandle()const
{
	return this->ground->GetModelHandle();
}