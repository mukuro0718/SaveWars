#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "Skydome.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Skydome::Skydome()
	: model(nullptr)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::SKYDOME));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Skydome::~Skydome()
{

}

void Skydome::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR position = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_POSITION"]);
	const VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_ROTATION"]);
	const VECTOR scale = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_SCALE"]);
	const VECTOR addPositionAndPosition = position + position;


	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->model->SetTransform(position, rotation, scale);
}

/// <summary>
/// �`��
/// </summary>
const void Skydome::Draw()const
{
	this->model->Draw();
}