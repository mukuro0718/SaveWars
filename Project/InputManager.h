//==============================================================
// @brief�@�C���v�b�g�}�l�[�W���[�N���X
// �v���W�F�N�g���Ŏg�p����p�b�h���͂�ێ�����
// �V���O���g���ō쐬���A�e�N���X�œ��͏�Ԃ��擾�ł���悤�ɂ���
//==============================================================
#pragma once
#include "Singleton.h"

class InputManager : public Singleton<InputManager>
{
public:
	friend class Singleton<InputManager>;
	
	virtual ~InputManager();//�f�X�g���N�^
	
	/*�A�i���O�X�e�B�b�N�\����*/
	struct InputAnalogStick
	{
		int XBuf = 0;
		int YBuf = 0;
	};
	
	/*�ړ��Ɏg�p�����ʓI�ȃL�[*/
	enum class ComKeysForMove
	{
		W,
		A,
		S,
		D,
		SPACE,	 //�㏸
		LCONTROL,//���~
	};

	/*���L�[*/
	enum class ArrowKey
	{
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

	/*�ÓI�萔*/
	//D_input
	static constexpr unsigned int PAD_X = PAD_INPUT_1;
	static constexpr unsigned int PAD_Y = PAD_INPUT_2;
	static constexpr unsigned int PAD_A = PAD_INPUT_3;
	static constexpr unsigned int PAD_B = PAD_INPUT_4;
	static constexpr unsigned int PAD_LB = PAD_INPUT_5;
	static constexpr unsigned int PAD_RB = PAD_INPUT_6;
	static constexpr unsigned int PAD_LT = PAD_INPUT_7;
	static constexpr unsigned int PAD_RT = PAD_INPUT_8;

	void Update();//�X�V

	/*getter*/
	const int GetNowPadState()const { return this->nowPad; }//pad��Ԃ�getter
	const int GetPrevPadState()const { return this->prevPad; }//pad��Ԃ�getter
	//const bool GetReturnKeyState()const { return this->isInputReturnKey; }//key��Ԃ�getter
	//const bool GetComKeysForMoveState(const ComKeysForMove _index)const { return this->moveKey[convMoveKey.at(_index)]; }//�ړ��L�[�̏�Ԃ�getter
	//const bool GetArrowKeyState(const ArrowKey _index)const { return this->arrowKey[convArrowKey.at(_index)]; }//���L�[�̏�Ԃ�getter
	const InputAnalogStick GetLStickState()const { return this->lStick; }//���X�e�B�b�N��Ԃ�getter
	const InputAnalogStick GetRStickState()const { return this->rStick; }//�E�X�e�B�b�N��Ԃ�getter
	//const DINPUT_JOYSTATE  GetButtonState()const { return this->button; }//�\���{�^�����͎擾�p�{�^���ϐ�
private:
	/*�ÓI�萔*/
	static constexpr int ARROW_KEY_NUM = 4;//���L�[�̐�
	static constexpr int COM_KEYS_FOR_MOVE = 6;//�ړ��L�[�̐�

	/*���������֐�*/
	InputManager();//�R���X�g���N�^
	void InputPadState();//PAD���͂̎擾
	void InputKeyState();//�L�[�{�[�h���͂̎擾

	/*�����o�ϐ�*/
	InputAnalogStick lStick;
	InputAnalogStick rStick;
	int nowPad;//�p�b�h���͕ϐ�
	int prevPad;//�p�b�h���͕ϐ�
};

