#include <DxLib.h>
#include "UseSTL.h"
#include "EnemyChanger.h"
#include "InputManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyChanger::EnemyChanger()
	: enemyType			(0)
	, inputInterval		(0)
	, isProvDecide		(false)
	, isFinalDecide		(false)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyChanger::~EnemyChanger()
{

}

/// <summary>
/// ������
/// </summary>
void EnemyChanger::Initialize()
{
	this->enemyType		= 0;
	this->selectType	= 0;
	this->inputInterval = 0;
	this->isProvDecide	= false;
	this->isFinalDecide = false;
}

/// <summary>
/// �X�V
/// </summary>
void EnemyChanger::Update()
{
	/*�����ŏI���肪�s���Ă����瑁�����^�[��*/
	if (this->isFinalDecide)return;

	/*�G�l�~�[�̎�ނ�I��*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& sound = Singleton<SoundManager>::GetInstance();
	if (this->inputInterval <= 0)
	{
		if (!this->isProvDecide)
		{
			if ((input.GetLStickState().YBuf < 0) || CheckHitKey(KEY_INPUT_UP))
			{
				sound.OnIsPlayEffect(SoundManager::EffectType::CHANGE_SELECT_ICON);
				this->enemyType--;
				int min = static_cast<int>(EnemyType::GOLEM);
				if (this->enemyType < min)
				{
					this->enemyType = min;
				}
				this->inputInterval = this->MAX_INPUT_INTERBVAL;
			}
			else if ((input.GetLStickState().YBuf > 0) || CheckHitKey(KEY_INPUT_DOWN))
			{
				sound.OnIsPlayEffect(SoundManager::EffectType::CHANGE_SELECT_ICON);
				this->enemyType++;
				int max = static_cast<int>(EnemyType::DRAGON);
				if (this->enemyType > max)
				{
					this->enemyType = max;
				}
				this->inputInterval = this->MAX_INPUT_INTERBVAL;
			}
		}
	}
	else
	{
		this->inputInterval--;
	}

	/*����*/
	int nowPad = input.GetNowPadState();
	int prevPad = input.GetPrevPadState();
	//�p�b�h��B�܂��̓L�[��E�������ꂽ��
	if ((!(prevPad & InputManager::PAD_B) && (nowPad & InputManager::PAD_B)) || CheckHitKey(KEY_INPUT_E))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::CONFIRMATION_SOUND);
		if (!this->isProvDecide)
		{
			this->isProvDecide = true;
		}
		else
		{
			this->isFinalDecide = true;
		}
	}
	//�p�b�h��A�܂��̓L�[��F�������ꂽ��
	else if ((nowPad & InputManager::PAD_A) || CheckHitKey(KEY_INPUT_F))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::CANCEL_SOUND);
		this->isProvDecide = false;
	}
}