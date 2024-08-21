#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "Satellite.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Satellite::Satellite()
	: model(nullptr)
	, angle(0.0f)
	, rotationAxis{ 0.0f,0.0f,0.0f }
	, positionOffset{ 0.0f,0.0f,0.0f }
	, targetDirection{ 0.0f,0.0f,0.0f }
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::SATELLITE));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Satellite::~Satellite()
{
	DeleteMemberInstance(this->model);
}

/// <summary>
/// ������
/// </summary>
void Satellite::Initialize(const float _angleOffset, const VECTOR _targetPosition, const VECTOR _positionOffset)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�g�p����ϐ��̏���*/
	this->positionOffset  = _positionOffset;
	const VECTOR position = _targetPosition + this->positionOffset;
	const VECTOR rotation = { 0.0f,0.0f,0.0f };
	const VECTOR scale	  = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_SCALE"]);

	this->targetDirection = VGet(1.0f, 0.0f, 0.0f);
	/*��]���̐ݒ�*/
	this->rotationAxis = { 0.0f,0.0f,0.0f };

	this->angle = _angleOffset;

	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->model->SetTransform(position, rotation, scale);
}

/// <summary>
/// �X�V
/// </summary>
void Satellite::Update(const bool _isLeft,const VECTOR _targetDirection, const VECTOR _targetPosition)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*�萔�l�ɑ��*/
	const float OFFSET	  = json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_OFFSET"];

	/*�g�p����ϐ��̏���*/
	this->positionOffset = VCross(VGet(0.0f, 1.0f, 0.0f), _targetDirection) * OFFSET;

	if (_isLeft)
	{
		this->positionOffset = this->positionOffset * -1.0f;
	}

	VECTOR position = _targetPosition + this->positionOffset;

	this->model->SetPosition(position);
}

/// <summary>
/// �`��
/// </summary>
const void Satellite::Draw()const
{
	VECTOR position = this->model->GetPosition();
	printfDx("SATELLITE_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	this->model->Draw();
}