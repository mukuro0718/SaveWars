//=====================================================================
// @brief �r�b�g�t���O�N���X
//=====================================================================
#pragma once
class BitFlag
{
public:
	void				SetFlag(const unsigned int _flag);		 //�t���O��on�ɂ���
	void				ClearFlag(const unsigned int _flag);	 //�t���O��off�ɂ���
	const bool			CheckFlag(const unsigned int _flag)const;//�t���O�̊m�F
	const unsigned int	GetFlag()const { return this->flags; }	 //�t���O�̎擾
private:
	unsigned int flags;//�r�b�g�t���O
};

