#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "MapManager.h"
#include "BossMap.h"
#include "Skydome.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
MapManager::MapManager()
	: ground(nullptr)
	, skydome(nullptr)
{
	this->ground  = new BossMap();
	this->skydome = new Skydome();

	Initialize();
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
/// �X�V
/// </summary>
void MapManager::Update() 
{
	this->ground->Update();
}

/// <summary>
/// �`��
/// </summary>
const void MapManager::Draw()const
{
	this->ground->Draw();
	this->skydome->Draw();
}