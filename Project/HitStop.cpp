#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "HitStop.h"
#include "InputManager.h"
#include "BitFlag.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
HitStop::HitStop()
	: time(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HitStop::~HitStop()
{

}

/// <summary>
/// ������
/// </summary>
void HitStop::Initialize()
{
	this->time = 0.0f;
}

/// <summary>
/// �q�b�g�X�g�b�v����
/// </summary>
bool HitStop::IsHitStop()
{
	/*���Ԃ��c���Ă�����true��Ԃ�*/
	if (this->time > 0)
	{
		this->time--;
		return true;
	}
	this->time = 0;
	return false;
}

/// <summary>
/// �`��
/// </summary>
const void HitStop::Draw()const
{
#ifdef _DEBUG
	if (this->time != 0)
	{
		printfDx("HIT_STOP\n");
	}
#endif // _DEBUG
}