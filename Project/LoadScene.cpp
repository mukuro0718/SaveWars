#include <DxLib.h>
#include "LoadScene.h"

const int LoadScene::FONT_COLOR = GetColor(255, 255, 255);

/// <summary>
/// �R���X�g���N�^
/// </summary>
LoadScene::LoadScene()
	: alpha(MIN_ALPHA)
	, isAddAlpha(false)
{
	Initialize();
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
LoadScene::~LoadScene()
{
	Finalize();
}
/// <summary>
/// ������
/// </summary>
void LoadScene::Initialize()
{
	SetFontSize(this->FONT_SIZE);
}
/// <summary>
/// �㏈��
/// </summary>
void LoadScene::Finalize()
{
}
/// <summary>
/// �X�V
/// </summary>
void LoadScene::Update()
{
	//�A���t�@�l�̑���
	if (this->isAddAlpha)
	{
		AddAlpha();
	}
	else
	{
		RemoveAlpha();
	}
}
/// <summary>
/// �`��
/// </summary>
const void LoadScene::Draw()const
{
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawString(this->TEXT_POS_X, this->TEXT_POS_Y, "Now Loading", this->FONT_COLOR);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->alpha);
}

/// <summary>
/// �A���t�@�̒ǉ�
/// </summary>
void LoadScene::AddAlpha()
{
	/*�A���t�@�̒ǉ�*/
	this->alpha += this->ADD_ALPHA;

	/*�A���t�@���ő�l�ȉ��������烊�^�[��*/
	if (this->alpha <= this->MAX_ALPHA) return;

	/*�ő�l���傫��������*/
	this->isAddAlpha = false;			//�ǉ��t���O������
	this->alpha		 = this->MAX_ALPHA;	//�A���t�@���ő�l�ɂ���
}

/// <summary>
/// �A���t�@�̍폜
/// </summary>
void LoadScene::RemoveAlpha()
{
	/*�A���t�@�̍폜*/
	this->alpha -= this->ADD_ALPHA;

	/*�A���t�@���ŏ��l�ȏゾ�����烊�^�[��*/
	if (this->alpha >= this->MIN_ALPHA) return;

	/*�ŏ��l��菬����������*/
	this->isAddAlpha = true;			//�ǉ��t���O�𗧂Ă�
	this->alpha		 = this->MIN_ALPHA;	//�A���t�@���ŏ��l�ɂ���
}