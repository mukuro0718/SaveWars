#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "LoadingAsset.h"
#include "BossMap.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossMap::BossMap()
	: Collidable(Collidable::Priority::STATIC, GameObjectTag::GROUND, GoriLib::ColliderData::Kind::PLANE, false)
	, modelHandle(-1)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
	
	/*�R���C�_�[�f�[�^�̍쐬*/
	auto planeColiderData = dynamic_cast<GoriLib::ColliderDataPlane*>(this->colliderData);
	planeColiderData->norm = Convert(json.GetJson(JsonManager::FileType::MAP)["MAP_NORM"]);
	planeColiderData->radius = json.GetJson(JsonManager::FileType::MAP)["MAP_SCALE"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossMap::~BossMap()
{
}

void BossMap::Initialize(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);	

	/*�R���C�_�[�̏�����*/
	Collidable::Initialize(_physics);

	/*���������̏�����*/
	this->rigidbody.Initialize(false);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
}
/// <summary>
/// �X�V
/// </summary>
void BossMap::Finalize(GoriLib::Physics* _physics)
{
	/*�����o�^�̉���*/
	Collidable::Finalize(_physics);
}
/// <summary>
/// �X�V
/// </summary>
void BossMap::Update(GoriLib::Physics* _physics)
{
}
void BossMap::OnCollide(const Collidable& _colider)
{
	std::string message = "�n�ʂ�";
	if (_colider.GetTag() == GameObjectTag::BOSS)
	{
		message += "�{�X";
	}
	else if (_colider.GetTag() == GameObjectTag::GROUND)
	{
		message += "�v���C���[";
	}

	message += "�Ɠ�������\n";
	printfDx(message.c_str());
}
/// <summary>
/// �`��
/// </summary>
const void BossMap::Draw()const
{
	MV1SetPosition(this->modelHandle, this->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->rigidbody.GetRotation());
	MV1SetScale(this->modelHandle, this->rigidbody.GetScale());
	MV1DrawModel(this->modelHandle);
}

