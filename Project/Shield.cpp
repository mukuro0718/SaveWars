#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "Shield.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Shield::Shield()
	: model(nullptr)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::SHIELD));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Shield::~Shield()
{

}

/// <summary>
/// ������
/// </summary>
void Shield::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = VGet(0.0f, 0.0f, 0.0f);
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SHIELD_ROTATION"]);
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SHIELD_SCALE"]);

	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->model->SetTransform(POSITION, ROTATION, SCALE);
}

/// <summary>
/// �X�V
/// </summary>
void Shield::Update(const VECTOR _targetPosition)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION_OFFSET = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SHIELD_POSITION_OFFSET"]);
	
	/*���W���o��*/
	const VECTOR POSITION = _targetPosition + POSITION_OFFSET;

	/*���W�Ɖ�]���̐ݒ�*/
	this->model->SetPosition(POSITION);
}

/// <summary>
/// �`��
/// </summary>
const void Shield::Draw()const
{
	this->model->Draw();
}