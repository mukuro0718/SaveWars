#include <DxLib.h>
#include "InputManager.h"

//InputManager* Singleton<InputManager>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
InputManager::InputManager()
	: pad(0)
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
	this->pad = GetJoypadInputState(DX_INPUT_PAD1);
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
	///*�ϐ��̏�����*/
	//for (int i = 0; i < moveKey.size(); i++)
	//{
	//	moveKey[i] = false;
	//}
	//for (int i = 0; i < arrowKey.size(); i++)
	//{
	//	arrowKey[i] = false;
	//}

	///*�G���^�[���͂̎擾*/
	//if (!this->isPrevInputReturnKey && CheckHitKey(KEY_INPUT_RETURN))
	//{
	//	this->isInputReturnKey = true;
	//	this->isPrevInputReturnKey = true;
	//}
	//else
	//{
	//	this->isInputReturnKey = false;
	//	if (!CheckHitKey(KEY_INPUT_RETURN))
	//	{
	//		this->isPrevInputReturnKey = false;
	//	}
	//}

	///*�ړ��L�[���͂̎擾*/
	//if (CheckHitKey(KEY_INPUT_W))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::W]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_D))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::D]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_S))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::S]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_A))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::A]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_SPACE))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::SPACE]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_LCONTROL))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::LCONTROL]] = true;
	//}

	///*���L�[���͂̎擾*/
	//if (CheckHitKey(KEY_INPUT_RIGHT))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::RIGHT]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_LEFT))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::LEFT]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::UP]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::DOWN]] = true;
	//}
}
