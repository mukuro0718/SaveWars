//=============================================================================================
// @brief �V�[���n��UI�N���X
// �^�C�g�����S�A�Q�[���I�[�o�[/�N���A
// ���U���g�̕`����s��
//=============================================================================================
#pragma once

class SceneUI abstract
{
public:
			 SceneUI(){}//�R���X�g���N�^
	virtual ~SceneUI(){}//�f�X�g���N�^

	virtual void		Initialize	() abstract;	  //������
	virtual	void		Update		() abstract;	  //�X�V
	virtual const void	Draw		() const abstract;//�`��
	virtual const bool	IsEnd		() const abstract;//�I�����邩
};

