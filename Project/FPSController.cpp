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
	, count(0)
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
	this->count		= 0;
	this->isDebug	= false;
	this->timeScale = this->NORMAL_TIME_SCALE;
}


void FPSController::CalcStartTime()
{
	if (this->count == 0)
	{
		this->startTime = GetNowCount();
	}
}
/// <summary>
/// FPS�̕���
/// </summary>
void FPSController::Average()
{
	/*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);

	/*�J�E���g���ڕWFPS�ɂȂ�����*/
	if (this->count == this->targetFPS)
	{
		//���݂̎������擾
		int nowTime = GetNowCount();
		//FPS�v�Z
		this->fps = static_cast<double>(this->timeScale / (nowTime - this->startTime) / this->targetFPS);
		//������
		this->startTime = 0;
		this->count = 0;
	}
	else
	{
		this->count++;
	}
}
const void FPSController::Draw()const
{
	/*�N���X�C���X�^���X�̎擾*/
	auto& debug = Singleton<Debug>::GetInstance();

	if (debug.IsShowDebugInfo(Debug::ItemType::FPS))
	{
		printfDx("FPS:%f\n", this->fps);
		printfDx("P:+ M:- TARGET_FPS:%d\n", this->targetFPS);
	}
}

/// <summary>
/// �ڕWFPS�ɂȂ�悤�ҋ@
/// </summary>
void FPSController::Wait()
{
	/*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
	auto& json = JsonManager::GetInstance();

	/*���݂̌o�ߎ���*/
	int elapsedTime = GetNowCount() - this->startTime;
	
	/*�ҋ@����*/
	int waitTime = this->count * this->timeScale / static_cast<int>(targetFPS) - elapsedTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}	
}
