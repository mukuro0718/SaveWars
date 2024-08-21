//=============================================
//���[�h��ʃN���X
// 5�̒��_��ێ����ADrawLine�Ő���`�悷��
// �܂����_�̈ʒu���ړ������A�����Ă��邱�Ƃ�\���B
//=============================================
#pragma once
#include "SceneBase.h"

class LoadScene : public SceneBase
{
public:
	LoadScene();//�R���X�g���N�^
	~LoadScene();//�f�X�g���N�^

	void		Initialize	() override;		//������
	void		Finalize	() override;		//�ŏI����
	void		Update		() override;		//�X�V
	const void	Draw		() const override;	//�`��
private:
	/*�ÓI�萔*/
	static constexpr int MAX_ALPHA  = 255;	//�ő�A���t�@
	static constexpr int MIN_ALPHA  = 0;	//�ŏ��A���t�@
	static constexpr int ADD_ALPHA  = 1;	//�A���t�@�̑����l
	static constexpr int TEXT_POS_X = 1620;	//������`�悷��X���W
	static constexpr int TEXT_POS_Y = 980;	//������`�悷��Y���W
	static constexpr int FONT_SIZE  = 32;	//�t�H���g�̃T�C�Y
	static const	 int FONT_COLOR;		//�t�H���g�̐F

	/*���������֐�*/
	void AddAlpha	();//�A���t�@�̒ǉ�
	void RemoveAlpha();//�A���t�@�̍폜

	/*�����o�ϐ�*/
	int  alpha;		//�A���t�@
	bool isAddAlpha;//�l���������Ă��邩(TRUE:�ǉ� FALSE:�폜 )
};

