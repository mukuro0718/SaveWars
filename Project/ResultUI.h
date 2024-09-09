//==============================================================================
// @brief ���U���gUI�N���X
//	�摜�E�e�L�X�g�̕`��ƍX�V�������s��
//==============================================================================
#pragma once

class SceneUI;
class ResultUI : public SceneUI
{
public:
	ResultUI();//�R���X�g���N�^
	~ResultUI();//�f�X�g���N�^

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