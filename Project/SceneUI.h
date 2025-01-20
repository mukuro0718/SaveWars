//=============================================================================================
// @brief �V�[���n��UI�N���X
// �^�C�g�����S�A�Q�[���I�[�o�[/�N���A
// ���U���g�̕`����s��
//=============================================================================================
#pragma once

class SceneUI abstract
{
public:
			 SceneUI();//�R���X�g���N�^
	virtual ~SceneUI(){}//�f�X�g���N�^

	virtual void		Initialize	() abstract;	  //������
	virtual	void		Update		() abstract;	  //�X�V
	virtual const void	Draw		() const abstract;//�`��
	virtual const bool	IsEnd		() const abstract;//�I�����邩
protected:
	/*�ÓI�萔*/
	static constexpr short MAX_ALPHA = 255;//�ő�A���t�@
	static constexpr short ALPHA_INCREASE = 2;//�ő�A���t�@
	static constexpr short ALPHA_REDUCTION = 5;//�ő�A���t�@
	static constexpr short MAX_X = 1920;//�ő�X
	static constexpr short MAX_Y = 1080;//�ő�Y
	
	/*�����o�ϐ�*/
	short alphaForTransition;//�J�ڗp�A���t�@
	bool isTransition;//�J�ڂ��邩
	bool isEndFadeInForTransition;//�J�ڂ��邩
};

