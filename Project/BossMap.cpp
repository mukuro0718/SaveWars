#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"
#include "LoadingAsset.h"
#include "BossMap.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossMap::BossMap()
	: collider(nullptr)
{
	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new ModelColliderData(ColliderData::Priority::STATIC, GameObjectTag::GROUND);
	
	/*���f���n���h���̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	modelColiderData.modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossMap::~BossMap()
{
}

/// <summary>
/// ������
/// </summary>
void BossMap::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/

	/*���W�b�h�{�f�B�̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	VECTOR position = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);
	VECTOR scale	= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);
	VECTOR rotation = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	rotation.y = rotation.y * (DX_PI_F / 180.0f);
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(position);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(scale);

	/*���f���̐ݒ�*/
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	modelColiderData.frameIndex = json.GetJson(JsonManager::FileType::MAP)["COLL_FRAME_INDEX"];
	MV1SetPosition(modelColiderData.modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(modelColiderData.modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale(modelColiderData.modelHandle, this->collider->rigidbody.GetScale());
	MV1SetFrameVisible(modelColiderData.modelHandle, modelColiderData.frameIndex, FALSE);
	MV1SetupCollInfo(modelColiderData.modelHandle, modelColiderData.frameIndex, 16, 16, 16);
}

/// <summary>
/// �㏈��
/// </summary>
void BossMap::Finalize()
{
	/*�����o�^�̉���*/
	DeleteMemberInstance(this->collider);
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
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	MV1DrawModel(modelColiderData.modelHandle);
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
const int BossMap::GetModelHandle()const
{ 
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	return modelColiderData.modelHandle;
}