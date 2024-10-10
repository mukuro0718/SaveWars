#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "LoadingAsset.h"
#include "Skydome.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Skydome::Skydome()
	: model(-1)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::SKYDOME));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Skydome::~Skydome()
{

}

/// <summary>
/// ������
/// </summary>
void Skydome::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_POSITION"]);
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_SCALE"]);
		  VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_ROTATION"]);
	rotation.y = rotation.y * (DX_PI_F / 180.0f);


	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->position	= POSITION;
	this->rotation	= rotation;
	this->scale		= SCALE;
}

/// <summary>
/// �X�V
/// </summary>
void Skydome::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*��]*/
	this->rotation.y += static_cast<float>(json.GetJson(JsonManager::FileType::MAP)["ADD_SKYDOME_ROTATE"]);
}

/// <summary>
/// �`��
/// </summary>
const void Skydome::Draw()const
{
	MV1SetPosition(this->model,this->position);
	MV1SetRotationXYZ(this->model,this->rotation);
	MV1SetScale(this->model,this->scale);
	MV1DrawModel(this->model);
}