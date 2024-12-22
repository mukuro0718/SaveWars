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

	/*�L�����Z���t���O�������Ă���Ƃ��ɍs����A�N�V�����Q*/
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::AVOID));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::COMBO_1));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::COMBO_2));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::COMBO_3));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::STRONG_ATTACK));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::DRAW_SWORD_1));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::DRAW_SWORD_2));
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
bool PlayerController::StateChanger(const bool _isCancelAction, const bool _isEndAction, const bool _isDrawSword, const CharacterData& _data)
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
	else if (_data.isHit && !_data.isInvinvible)
	{
		//�K�[�h���Ă�����
		if (_data.isGuard)
		{
			nextState = static_cast<int>(PlayerState::BLOCK_STAGGER);
		}
		else
		{
			//���A�N�V����
			switch (_data.reactionType)
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
	else if (this->nowState == static_cast<int>(PlayerState::KNOCK_DOWN))
	{
		nextState = static_cast<int>(PlayerState::KNOCK_UP);
	}
	/*�K�[�h*/
	//�X�^�~�i�͑���Ă��邩
	else if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]) && input.GetNowPadState() & InputManager::PAD_RT)
	{
		if (_isDrawSword)
		{
			nextState = static_cast<int>(PlayerState::BLOCK);
		}
		else
		{
			nextState = static_cast<int>(PlayerState::DRAW_SWORD_2);
		}
	}

	/*��U��*/
	else if (input.GetNowPadState() & InputManager::PAD_B)
	{
		if (_isDrawSword)
		{
			switch (this->nowState)
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
		else
		{
			nextState = static_cast<int>(PlayerState::DRAW_SWORD_2);
		}
	}

	/*���U��*/
	else if (input.GetNowPadState() & InputManager::PAD_Y)
	{
		if (_isDrawSword)
		{
			nextState = static_cast<int>(PlayerState::STRONG_ATTACK);
		}
		else
		{
			nextState = static_cast<int>(PlayerState::DRAW_SWORD_2);
		}
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
			if (!_isDrawSword)
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
				nextState = static_cast<int>(PlayerState::DRAW_SWORD_1);
			}
		}
		else
		{
			//����
			nextState = static_cast<int>(PlayerState::WALK);
		}
	}

	///*���킾��*/
	//else if (input.GetNowPadState() & InputManager::PAD_RB && _isDrawSword)
	//{
	//	nextState = static_cast<int>(PlayerState::DRAW_SWORD_1);
	//}
	else
	{
		nextState = static_cast<int>(PlayerState::IDLE);
	}

	/*�A�N�V�������I�����Ă��邩�A�I�����ꂽ��Ԃ̗D�揇�ʂ�������*/
	if (_isEndAction || this->priority[nextState] > this->priority[this->nowState])
	{
		this->prevState = this->nowState;
		this->nowState = nextState;
		return true;
	}

	/*�L�����Z���t���O�������Ă��邩�L�����Z���ł���A�N�V����*/
	else if (_isCancelAction)
	{
		for (int state : this->stateTheIsCancel)
		{
			if (state == nextState)
			{
				this->prevState = this->nowState;
				this->nowState = nextState;
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// �K�v�ȏ���X�^�~�i�ƌ��݂̃X�^�~�i�̎c�ʂ��ׂăA�N�V�������\�Ȃ�true��Ԃ�
/// </summary>
const bool PlayerController::CanAction(const float _stamina, const float _staminaConsumed)const
{
	/*�X�^�~�i�̏���ʂ����݂̃X�^�~�i�̑��ʂ����������false��Ԃ�*/
	float staminaConsumed = _staminaConsumed * -1.0f;
	if (staminaConsumed > _stamina)return false;
	return true;
}
