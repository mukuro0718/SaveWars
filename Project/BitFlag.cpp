#include "BitFlag.h"

/// <summary>
/// �����̃t���O��ON�ɂȂ��Ă��邩�ǂ���
/// </summary>
const bool BitFlag::CheckFlag(const unsigned int _flag)const
{
	//���������̃t���O�������Ă��Ȃ�������false��Ԃ�
	if (!(this->flags & _flag)) return false;
	return true;
}

/// <summary>
/// �t���O��ON�ɂ���
/// </summary>
void BitFlag::SetFlag(const unsigned int _flag)
{
	this->flags |= _flag;
}

/// <summary>
/// �t���O��OFF�ɂ���
/// </summary>
void BitFlag::ClearFlag(const unsigned int _flag)
{
	this->flags &= ~_flag;
}
