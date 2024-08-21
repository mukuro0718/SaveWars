#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "Debug.h"
#include "InputManager.h"
#include "BitFlag.h"

const int Debug::COLOR_WHITE = GetColor(255, 255, 255);//���F
const int Debug::COLOR_BLACK = GetColor(0, 0, 0);//���F

/// <summary>
/// �R���X�g���N�^
/// </summary>
Debug::Debug()
	: flags(nullptr)
	, currentlySelectedItem(0)
	, previousSelectedItem(0)
	, isShow(false)
{
	this->flags = new BitFlag();
	;
	/*�֐�map�̐ݒ�*/
	auto cameraSet	 = [this]() { this->flags->SetFlag (this->CAMERA); };
	auto cameraClear = [this]() { this->flags->ClearFlag(this->CAMERA); };
	auto playerSet	 = [this]() { this->flags->SetFlag  (this->PLAYER); };
	auto playerClear = [this]() { this->flags->ClearFlag(this->PLAYER); };
	auto fpsSet		 = [this]() { this->flags->SetFlag  (this->FPS); };
	auto fpsClear	 = [this]() { this->flags->ClearFlag(this->FPS); };
	AddItemFunction(static_cast<int>(ItemType::CAMERA), cameraSet, cameraClear);
	AddItemFunction(static_cast<int>(ItemType::PLAYER), playerSet, playerClear);
	AddItemFunction(static_cast<int>(ItemType::FPS), fpsSet	  , fpsClear);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Debug::~Debug() 
{

}

/// <summary>
/// ������
/// </summary>
void Debug::Initialize()
{
	/*���ׂĂ��P�̃}�X�N�r�b�g���g���āA���ׂẴr�b�g������*/
	this->flags->ClearFlag(this->MASK_ALL_FLAGS);
	this->currentlySelectedItem = 0;
	this->previousSelectedItem = this->currentlySelectedItem;
	this->isShow = false;
}

/// <summary>
/// �X�V
/// </summary>
void Debug::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = InputManager::GetInstance();

	/*PAD���͂̎擾*/
	int pad = input.GetPadState();
	
	/*�f�o�b�N���ڂ̕\������*/
	this->isShow = IsShow();

	/*�I�����ڂ̕ύX*/
	ChangeSelectedItem();

	/*on/off�؂�ւ�*/
	ChangeFlagsState();
}

/// <summary>
/// �`��
/// </summary>
const void Debug::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = JsonManager::GetInstance();
	
	/*json�f�[�^��萔�^�ɑ��*/
	const int MENU_POS_X   = json.GetJson(JsonManager::FileType::DEBUG)["LOGO_X"];
	const int MENU_POS_Y   = json.GetJson(JsonManager::FileType::DEBUG)["LOGO_Y"];
	const int CURSOR_POS_X = json.GetJson(JsonManager::FileType::DEBUG)["CURSOR_X"];
	const int CURSOR_POS_Y = json.GetJson(JsonManager::FileType::DEBUG)["CURSOR_Y"];
	const int BACK_LEFT_X  = json.GetJson(JsonManager::FileType::DEBUG)["BACKGROUND_LEFT_X"];
	const int BACK_LEFT_Y  = json.GetJson(JsonManager::FileType::DEBUG)["BACKGROUND_LEFT_Y"];
	const int BACK_RIGHT_X = json.GetJson(JsonManager::FileType::DEBUG)["BACKGROUND_RIGHT_X"];
	const int BACK_RIGHT_Y = json.GetJson(JsonManager::FileType::DEBUG)["BACKGROUND_RIGHT_Y"];
	const int ALPHA		   = json.GetJson(JsonManager::FileType::DEBUG)["BACKGROUND_ALPHA"];
	const int MAX_ALPHA    = json.GetJson(JsonManager::FileType::DEBUG)["MAX_ALPHA"];
	const int TEXT_SPACE   = json.GetJson(JsonManager::FileType::DEBUG)["TEXT_SPACE"];
	const vector<string> FIELD_NAME = json.GetJson(JsonManager::FileType::DEBUG)["ITEM_NAME"];
	
	if (this->isShow)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
		DrawBox(BACK_LEFT_X, BACK_LEFT_Y, BACK_RIGHT_X, BACK_RIGHT_Y, this->COLOR_BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MAX_ALPHA);
		DrawString(MENU_POS_X, MENU_POS_Y, "DEBUG_MENU",this->COLOR_WHITE);
		DrawString(CURSOR_POS_X, CURSOR_POS_Y + TEXT_SPACE * this->currentlySelectedItem, "=>", this->COLOR_WHITE);
		for (int i = 0; i < FIELD_NAME.size(); i++)
		{
			DrawItem(MENU_POS_X, MENU_POS_Y + (TEXT_SPACE * (i + 1)), GetBitFlagInMap(i), FIELD_NAME[i]);
		}
	}
}


const bool Debug::IsShow()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = InputManager::GetInstance();

	/*PAD���͂̎擾*/
	int pad = input.GetPadState();

	/*���j���[���J����Ă��Ȃ�������*/
	if (!this->isShow)
	{
		//�R���g���[���[��start�{�^���܂���O�L�[��������Ă��Ȃ�������false��Ԃ�
		if (!(pad & PAD_INPUT_12) && !(CheckHitKey(KEY_INPUT_O))) return false;
	}
	else
	{
		//�R���g���[���[��B�{�^���܂���B�L�[��������Ă�����false��Ԃ�
		if (pad & PAD_INPUT_4 || CheckHitKey(KEY_INPUT_B)) return false;
	}

	/*��L�ȊO��������true��Ԃ�*/
	return true;
}

/// <summary>
/// ���ڂ̕\��
/// </summary>
const void Debug::DrawItem(const int _x, const int _y, const unsigned int _checkFlag, const std::string _fieldName)const
{
	/*���ڂ̖��O��string�^�ɑ��*/
	string text = _fieldName;

	/*�t���O�̏�Ԃ��m�F*/
	if (this->flags->CheckFlag(_checkFlag))
	{
		//ON��ǉ�����
		text += "ON";
	}
	else
	{
		//OFF��ǉ�����
		text += "OFF";
	}

	/*�e�L�X�g�̕\��*/
	DrawString(_x, _y, text.c_str(), this->COLOR_WHITE);
	
	/*string�^���J������*/
	text.clear();
}

/// <summary>
/// �I������Ă��鍀�ڂ̕ύX
/// </summary>
void Debug::ChangeSelectedItem()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json  = JsonManager::GetInstance();
	auto& input = InputManager::GetInstance();

	/*�萔�^�ɑ��*/
	const int Y_BUF		= input.GetLStickState().YBuf;							 //�㉺����
	const int ITEM_NUM	= json.GetJson(JsonManager::FileType::DEBUG)["ITEM_NUM"];//���ڂ̐�
	
	/*�㉺���͂��Ȃ�*/
	if (Y_BUF == 0 && CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0)
	{
		//�ȑO�ƍ��̍��ڂ���v���Ă����烊�^�[����Ԃ�
		if (this->previousSelectedItem == this->currentlySelectedItem) return;
		/*���ڂ���v������*/
		this->previousSelectedItem = this->currentlySelectedItem;
	}
	else
	{
		//�ȑO�ƍ��̍��ڂ���v���Ă��Ȃ������烊�^�[����Ԃ�
		if (this->previousSelectedItem != this->currentlySelectedItem) return;

		//����͂���������
		if (Y_BUF < 0 || CheckHitKey(KEY_INPUT_UP))
		{
			this->currentlySelectedItem--;
			if (this->currentlySelectedItem < 0)
			{
				this->currentlySelectedItem = 0;
			}
		}
		//�����͂���������
		else if(Y_BUF > 0 || CheckHitKey(KEY_INPUT_DOWN))
		{
			this->currentlySelectedItem++;
			if (this->currentlySelectedItem > ITEM_NUM)
			{
				this->currentlySelectedItem = ITEM_NUM;
			}
		}
	}
}

void Debug::ChangeFlagsState()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = InputManager::GetInstance();

	/*�萔�^�ɑ��*/
	const int X_BUF = input.GetLStickState().XBuf;//���E����

	/*���͂��Ȃ���΃��^�[����Ԃ�*/
	if (X_BUF == 0 && CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0)return;

	/*�E����*/
	if (X_BUF > 0 || CheckHitKey(KEY_INPUT_RIGHT))
	{
		this->itemFunctionMap[this->currentlySelectedItem].set();
	}
	/*������*/
	else if (X_BUF < 0 || CheckHitKey(KEY_INPUT_LEFT))
	{
		this->itemFunctionMap[this->currentlySelectedItem].clear();
	}

}

/// <summary>
/// ���ڂ��Ƃ̊֐��̒ǉ�
/// </summary>
void Debug::AddItemFunction(const int _item, const FlagsState _set, const FlagsState _clear)
{
	FlagsStateSet add;
	add.set = _set;
	add.clear = _clear;
	this->itemFunctionMap.emplace(_item, add);
}

/// <summary>
/// �}�b�v���̃r�b�g�t���O���擾
/// </summary>
/// <returns></returns>
const unsigned int Debug::GetBitFlagInMap(const int _itemName)const
{
	switch (_itemName)
	{
	case static_cast<int>(ItemType::CAMERA):
		return this->CAMERA;
	case static_cast<int>(ItemType::PLAYER):
		return this->PLAYER;
	case static_cast<int>(ItemType::FPS):
		return this->FPS;
	}
	return 0;
}
/// <summary>
/// �J�����̃f�o�b�N�@�\��ON�ɂȂ��Ă��邩�ǂ���
/// </summary>
const bool Debug::CheckCameraFlag()const 
{ 
	return this->flags->CheckFlag(this->CAMERA);
}
