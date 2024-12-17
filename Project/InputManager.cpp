#include <DxLib.h>
#include "InputManager.h"

//InputManager* Singleton<InputManager>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
InputManager::InputManager()
	: nowPad(0)
	, prevPad(0)
	, lStick{0,0}
	, rStick{0,0}
{
	
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
InputManager::~InputManager()
{

}
/// <summary>
/// PAD���͂̎擾
/// </summary>
void InputManager::InputPadState()
{
	/*�{�^�����͂̎�t*/
	this->prevPad = this->nowPad;
	this->nowPad = GetJoypadInputState(DX_INPUT_PAD1);
	/*���X�e�B�b�N���͂̎擾*/
	GetJoypadAnalogInput(&this->lStick.XBuf, &this->lStick.YBuf, DX_INPUT_PAD1);
	/*�E�X�e�B�b�N�̓���*/
	GetJoypadAnalogInputRight(&this->rStick.XBuf, &this->rStick.YBuf, DX_INPUT_PAD1);
}
void InputManager::Update()
{
	InputPadState();
	InputKeyState();
}
/// <summary>
/// �L�[�{�[�h���͂̎擾
/// </summary>
void InputManager::InputKeyState()
{
}
/// <summary>
/// �}�E�X���͂̎擾
/// </summary>
void InputManager::InputMouseState()
{
}