//=====================================================
// @brief �V�[���x�[�X�N���X
// �e�V�[���ŕK�������������֐����������z�֐��Ŏ���
//=====================================================
#pragma once

class SceneBase abstract
{
public:
	SceneBase();			//�R���X�g���N�^
	virtual ~SceneBase();	//�f�X�g���N�^

	/*�������z�֐��i�p����Ŏ����j*/
	virtual void		Initialize	() abstract;		//������
	virtual void		Finalize	() abstract;		//�㏈��
	virtual void		Update		() abstract;		//�X�V
	virtual const void	Draw		() const abstract;	//�`��
};

