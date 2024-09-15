#include <DxLib.h>
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

	fps = 0.0f;
	startTime = 0;
	count = 0;
	targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);
}

/// <summary>
/// FPS�̕���
/// </summary>
void FPSController::Average()
{
	UpdateTargetFPS();

	/*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�J�E���g���O��������X�^�[�g�^�C����������*/
	if (this->count == 0)
	{
		this->startTime = GetNowCount();
	}

	/*�J�E���g���ڕWFPS�ɂȂ�����*/
	if (this->count == targetFPS)
	{
		//���݂̎������擾
		int nowTime = GetNowCount();
		//FPS�v�Z
		this->fps = 1000.0 / (static_cast<double>((nowTime - this->startTime)) / targetFPS);
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

	if (debug.CheckFPSFlag())
	{
		printfDx("FPS:%f\n", this->fps);
		printfDx("P:+ M:- TARGET_FPS:%d\n", this->targetFPS);
	}
}

/// <summary>
/// targetFPS�̍X�V
/// </summary>
void FPSController::UpdateTargetFPS()
{
	/*�N���X�C���X�^���X�̎擾*/
	auto& debug = Singleton<Debug>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	if (debug.CheckFPSFlag())
	{
		if (CheckHitKey(KEY_INPUT_P))
		{
			this->targetFPS++;
			if (this->targetFPS >= 60)
			{
				this->targetFPS = 60;
			}
		}
		if (CheckHitKey(KEY_INPUT_M))
		{
			this->targetFPS--;
			if (this->targetFPS <= 0)
			{
				this->targetFPS = 0;
			}
		}
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
	int waitTime = this->count * 1000 / static_cast<int>(targetFPS) - elapsedTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
}
