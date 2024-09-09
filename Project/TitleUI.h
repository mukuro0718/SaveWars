//==============================================================================
// @brief �^�C�g��UI�N���X
// �^�C�g���摜�̕`��ƍX�V�������s��
//==============================================================================
#pragma once

class SceneUI;
class TitleUI : public SceneUI
{
public:
	TitleUI();//�R���X�g���N�^
	~TitleUI();//�f�X�g���N�^

	void Initialize() override;
	void Update() override;
	const void Draw()const override;
private:

	/*�����o�ϐ�*/
	//�摜�ϐ�
	Image logo;
	Image pressA;
};

/*
�^�C�g�����S

alpha = 0 ���� MAX_ALPHA �܂Ŗ��t���[��+1����
alpha ���������Ƀv���C���[�� X ���������� alpha ����C�� MAX_ALPHA �ɂ���
*/

/*
pressA

�^�C�g�����S�� alpha �� MAX_ALPHA �ɓ��B������
alpha = 0 ���� MAX_ALPHA �܂Ŗ��t���[��+1����
MAX_ALPHA �ɓ��B�����玟�� alpha �� 0 �ɂȂ�܂Ŗ��t���[�� -1 ������������

A�{�^���������ꂽ���C�� alpha �� MAX_ALPHA �ɂ�����A
alpha �� 0 �ɂȂ�܂Ŗ��t���[�� -1 ������������
*/