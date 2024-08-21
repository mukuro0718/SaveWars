#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "BossMap.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossMap::BossMap()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossMap::~BossMap()
{
	DeleteMemberInstance(this->model);
}

void BossMap::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);	
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);


	this->model->SetTransform(POSITION, ROTATION, SCALE);
}

/// <summary>
/// �X�V
/// </summary>
void BossMap::Update()
{
}

/// <summary>
/// �`��
/// </summary>
const void BossMap::Draw()const
{
	this->model->Draw();
}

