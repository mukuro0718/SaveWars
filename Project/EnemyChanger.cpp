#include <DxLib.h>
#include "EnemyChanger.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyChanger::EnemyChanger()
	: enemyType				(0)
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
	this->enemyType = static_cast<int>(EnemyType::GOLEM);
	this->inputInterval = 0;
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
	if (this->inputInterval <= 0)
	{
		if (input.GetLStickState().YBuf < 0)
		{
			this->enemyType--;
			int min = static_cast<int>(EnemyType::GOLEM);
			if (this->enemyType < min)
			{
				this->enemyType = min;
			}
			this->inputInterval = this->MAX_INPUT_INTERBVAL;
		}
		else if (input.GetLStickState().YBuf > 0)
		{
			this->enemyType++;
			int max = static_cast<int>(EnemyType::TUTORIAL);
			if (this->enemyType > max)
			{
				this->enemyType = max;
			}
			this->inputInterval = this->MAX_INPUT_INTERBVAL;
		}
	}
	else
	{
		this->inputInterval--;
	}

	/*����*/
	int nowPad = input.GetNowPadState();
	int prevPad = input.GetPrevPadState();
	if (!(prevPad & InputManager::PAD_A) && (nowPad & InputManager::PAD_A))
	{
		if (!this->isProvDecide)
		{
			this->isProvDecide = true;
		}
		else
		{
			this->isFinalDecide = true;
		}
	}
	else if (nowPad & InputManager::PAD_B)
	{
		this->isProvDecide = false;
	}
}