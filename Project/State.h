//=============================================================
// @brief ��{�X�e�[�g�N���X
//=============================================================
#pragma once
class State abstract
{
public:
			 State(){}//�R���X�g���N�^
	virtual ~State(){}//�f�X�g���N�^

	virtual void Entry () abstract;//�o�^
	virtual void Update() abstract;//�X�V
	virtual void Exit  () abstract;//����
	virtual int  GetId () abstract;//ID�̎擾
};

