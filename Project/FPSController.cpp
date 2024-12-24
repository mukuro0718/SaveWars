#include "DxLib.h"
#include "UseJson.h"
#include "FPSController.h"
#include "Debug.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
FPSController::FPSController()
	: fps(0.0f)
	, startTime(0)
	, targetFPS(0)
{
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FPSController::~FPSController()
{

}

/// <summary>
/// ������
/// </summary>
void FPSController::Initialize()
{
	/*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);
	this->fps		= 0.0f;
	this->startTime = 0;
	this->timeScale = this->NORMAL_TIME_SCALE;
	this->frameCount = 0;
	this->lastTime = GetNowCount();
}

/// <summary>
/// �J�n���Ԃ̌v�Z
/// </summary>
void FPSController::CalcStartTime()
{
	this->startTime = GetNowCount();
}
/// <summary>
/// FPS�̍X�V
/// </summary>
void FPSController::Update()
{
	/*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ڕWFPS*/
	this->targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);

	/*���݂̃t���[������*/
	int frameTime = GetNowCount() - this->startTime;

	/*�ڕW�̃t���[������*/
	int targetFrameTime = this->timeScale / targetFPS;

	/*���݂̃t���[�����Ԃ��ڕW�̃t���[�����Ԃ������������*/
	if (frameTime < targetFrameTime)
	{
		//�ڕW�̃t���[�����ԂɂȂ�܂őҋ@����
		Sleep(targetFrameTime - frameTime);
	}

	// FPS�̌v�Z
	this->frameCount++;
	int currentTime = GetNowCount();
	// 1�b���ƂɍX�V
	if (currentTime - this->lastTime >= this->timeScale)
	{ 
		this->fps = (this->frameCount * static_cast<double>(this->timeScale)) / (currentTime - this->lastTime);
		this->lastTime = currentTime;
		this->frameCount = 0;
	}
}

/// <summary>
/// �`��
/// </summary>
const void FPSController::Draw()const
{
	/*�N���X�C���X�^���X�̎擾*/
	auto& debug = Singleton<Debug>::GetInstance();

	if (debug.IsShowDebugInfo(Debug::ItemType::FPS))
	{
		printfDx("FPS:%f\n", this->fps);
	}
}
