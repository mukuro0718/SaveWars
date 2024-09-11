#include <DxLib.h>
#include "CameraManager.h"
#include "DeleteInstance.h"
#include "Camera.h"

//CameraManager* Singleton<CameraManager>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
CameraManager::CameraManager()
	:camera(nullptr)
{
	this->camera = new Camera();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CameraManager::~CameraManager()
{
	DeleteMemberInstance(this->camera);
}

/// <summary>
/// ������
/// </summary>
void CameraManager::Initialize()
{
	this->camera->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void CameraManager::Update()
{
	this->camera->Update();
}

/// <summary>
/// ���݂̃J�����̍��W
/// </summary>
const VECTOR CameraManager::GetNowCameraPosition()const
{
	return this->camera->GetNowPosition();
}

/// <summary>
/// ���݂̃J�����̒����_
/// </summary>
const VECTOR CameraManager::GetNowCameraTarget()const
{
	return this->camera->GetNowTarget();
}

/// <summary>
/// �`��
/// </summary>
const void CameraManager::Draw()const
{
	this->camera->Draw();
}

/// <summary>
/// ���݂̃J�����̒����_
/// </summary>
const VECTOR CameraManager::GetCameraDirection()const
{
	return this->camera->GetDirection();
}
