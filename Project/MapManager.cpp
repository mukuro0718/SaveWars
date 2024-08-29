#include <DxLib.h>
#include "UseSTL.h"
#include "GoriLib.h"
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
void MapManager::Initialize(GoriLib::Physics* _physics)
{
	this->ground->Initialize(_physics);
	this->skydome->Initialize();
}

/// <summary>
/// �㏈��
/// </summary>
void MapManager::Finalize(GoriLib::Physics* _physics)
{
	this->ground->Finalize(_physics);
}

/// <summary>
/// �X�V
/// </summary>
void MapManager::Update(GoriLib::Physics* _physics)
{
	this->ground->Update(_physics);
}

/// <summary>
/// �ړ��x�N�g���̕␳
/// </summary>
void MapManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->ground->OnCollide(_colider);
}

/// <summary>
/// �`��
/// </summary>
const void MapManager::Draw()const
{
	this->ground->Draw();
	this->skydome->Draw();
}