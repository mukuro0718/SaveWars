#include <DxLib.h>
#include "UseSTL.h"
#include "HitStop.h"
#include "HitStopManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
HitStop::HitStop()
	: type		(static_cast<int>(Type::NONE))
	, time		(0)
	, slowFactor(this->SLOW_MOTION_FACTOR)
	, delay		(0)
{
	/*�}�l�[�W���[�ɓo�^*/
	auto& manager = Singleton<HitStopManager>::GetInstance();
	manager.Entry(*this);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HitStop::~HitStop()
{
	/*�}�l�[�W���[����폜*/
	auto& manager = Singleton<HitStopManager>::GetInstance();
	manager.Exit(*this);
}
/// <summary>
/// ������
/// </summary>
void HitStop::Initialize()
{
	this->type		 = static_cast<int>(Type::NONE);
	this->time		 = 0;
	this->slowFactor = this->SLOW_MOTION_FACTOR;
}
/// <summary>
/// �X�V
/// </summary>
void HitStop::Update()
{
	/*type��NONE�������瑁�����^�[��*/
	if (this->type == static_cast<int>(Type::NONE))return;

	/*�҂����Ԃ��c���Ă����瑁�����^�[��*/
	if (this->delay > 0)
	{
		this->delay--;
		return;
	}

	/*���Ԃ�����������*/
	this->time--;

	/*�O�����ɂȂ�����type��NONE�ɂ���*/
	if (this->time < 0)
	{
		this->type = static_cast<int>(Type::NONE);
	}
}


/// <summary>
/// �q�b�g�X�g�b�v����
/// </summary>
bool HitStop::IsHitStop()
{
	/*�҂����Ԃ��c���Ă����瑁�����^�[��*/
	if (this->delay != 0) return false;

	/*type��STOP��������*/
	if (this->type == static_cast<int>(Type::STOP))
	{
		return true;
	}

	/*type��SLOW��������*/
	else if (this->type == static_cast<int>(Type::SLOW))
	{
		if (this->time % (int)(1.0f / this->slowFactor) == 0)
		{
			return false;
		}
		return true;
	}
	return false;
}

/// <summary>
/// �q�b�g�X�g�b�v�̐ݒ�
/// </summary>
void HitStop::SetHitStop(const float _time, const int _type, const int _delay, const float _slowFactor)
{
	this->type		 = _type;
	this->time		 = _time;
	this->slowFactor = _slowFactor;
	this->delay		 = _delay;
}