//=================================================
// @brief ��ʂ̐ݒ�N���X
// window/fullScreen�̐؂�ւ�
// �J���[�r�b�g�̐ݒ�
// ��ʃT�C�Y�̐ݒ�
//=================================================

#pragma once
#include "Singleton.h"

class ScreenSetup : public Singleton<ScreenSetup>
{
public:
	friend class Singleton<ScreenSetup>;
	
	int	Initialize	();//������
protected:
	/*���������֐�*/
	ScreenSetup();
	virtual ~ScreenSetup();//�f�X�g���N�^
};

