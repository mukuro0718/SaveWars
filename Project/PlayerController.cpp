#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ReactionType.h"
#include "CharacterData.h"
#include "PlayerController.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerController::PlayerController()
	: nowState(0)
	, prevState(0)
{
	/*�D��t���O��t����*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	vector<int> priority = json.GetJson(JsonManager::FileType::PLAYER)["ACTION_PRIORITY"];
	this->priority = priority;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerController::~PlayerController()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerController::Initialize()
{
	this->nowState = 0;
	this->prevState = 0;
}

/// <summary>
/// ��ԕύX�N���X
/// </summary>
void PlayerController::StateChanger(const bool _isEndAction, const CharacterData& _data)
{
	int nextState = -1;

	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager>  ::GetInstance();

	/*HP��0�ȉ��Ȃ��Ԃ�DEATH�ɂ���*/
	if (_data.hp <= 0 )
	{
		nextState = static_cast<int>(PlayerState::DEATH);
	}

	/*�U�����q�b�g���Ă����烊�A�N�V�����̎�ޕʂ̏�Ԃɂ���*/
	else if (_data.isHit)
	{
		//�K�[�h���Ă�����
		if (_data.isGuard)
		{
			nextState = static_cast<int>(PlayerState::BLOCK_STAGGER);
		}
		else
		{
			//���A�N�V����
			switch (_data.playerReaction)
			{
			case static_cast<int>(Gori::PlayerReactionType::NORMAL):
				nextState = static_cast<int>(PlayerState::STAGGER);
				break;
			case static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL):
				nextState = static_cast<int>(PlayerState::STAGGER);
				break;
			case static_cast<int>(Gori::PlayerReactionType::BLOW_BIG):
				nextState = static_cast<int>(PlayerState::KNOCK_DOWN);
				break;
			}
		}
	}

	/*�K�[�h*/
	//�X�^�~�i�͑���Ă��邩
	else if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]) && input.GetNowPadState() & InputManager::PAD_RT)
	{
		nextState = static_cast<int>(PlayerState::BLOCK);
	}

	/*��U��*/
	else if (input.GetNowPadState() & InputManager::PAD_B)
	{
		switch (this->prevState)
		{
		case static_cast<int>(PlayerState::COMBO_1):
			nextState = static_cast<int>(PlayerState::COMBO_2);
			break;
		case static_cast<int>(PlayerState::COMBO_2):
			nextState = static_cast<int>(PlayerState::COMBO_3);
			break;
		default:
			nextState = static_cast<int>(PlayerState::COMBO_1);
			break;
		}
	}

	/*���U��*/
	else if (input.GetNowPadState() & InputManager::PAD_Y)
	{
		nextState = static_cast<int>(PlayerState::STRONG_ATTACK);
	}

	/*���*/
	else if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]) && input.GetNowPadState() & InputManager::PAD_A)
	{
		nextState = static_cast<int>(PlayerState::AVOID);
	}

	/*��*/
	else if (input.GetNowPadState() & InputManager::PAD_X)
	{
		nextState = static_cast<int>(PlayerState::HEAL);
	}

	/*�ړ�*/
	else if (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0)
	{
		if (input.GetNowPadState() & InputManager::PAD_RB)
		{
			//�_�b�V��
			if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
			{
				nextState = static_cast<int>(PlayerState::RUN);
			}
			//�X�^�~�i�؂�
			else
			{
				nextState = static_cast<int>(PlayerState::RUNNING_OUT_OF_STAMINA);
			}
		}
		else
		{
			//����
			nextState = static_cast<int>(PlayerState::WALK);
		}
	}
	else
	{
		nextState = static_cast<int>(PlayerState::IDLE);
	}

	/*�A�N�V�������I�����Ă��邩�A�I�����ꂽ��Ԃ̗D�揇�ʂ�������*/
	if (_isEndAction || this->priority[nextState] > this->priority[this->nowState])
	{
		this->prevState = this->nowState;
		this->nowState = nextState;
	}
}

/// <summary>
/// �K�v�ȏ���X�^�~�i�ƌ��݂̃X�^�~�i�̎c�ʂ��ׂăA�N�V�������\�Ȃ�true��Ԃ�
/// </summary>
const bool PlayerController::CanAction(const float _stamina, const float _staminaConsumed)const
{
	/*�X�^�~�i�̏���ʂ����݂̃X�^�~�i�̑��ʂ����������false��Ԃ�*/
	if (_staminaConsumed > _stamina)return false;
	return true;
}
