#include <DxLib.h>
#include "UseSTL.h"
#include "Image.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Image::Image(int _imageHandle)
	: imageHandle(_imageHandle)
	, alpha		 (0)
	, isAddAlpha (false)
	, interval	 (0)
{

}

/// <summary>
/// �`��
/// </summary>
const void Image::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->position[0], this->position[1], this->position[2], this->position[3], this->imageHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}

/// <summary>
/// ���W�̐ݒ�
/// </summary>
void Image::SetPosition(std::vector<int> _position)
{
	this->position = _position;
}

/// <summary>
/// �t�F�[�h�C��
/// </summary>
void Image::FadeIn(const int _addAlpha)
{
	/*�A���t�@�����t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isAddAlpha)return;

	/*�A���t�@�̑���*/
	this->alpha += _addAlpha;

	/*�ő�𒴂��Ȃ��悤�ɂ���*/
	if (this->alpha >= this->MAX_ALPHA)
	{
		this->alpha = this->MAX_ALPHA;
		this->isAddAlpha = false;
	}
}

/// <summary>
/// �t�F�[�h�A�E�g
/// </summary>
void Image::FadeOut(const int _decreaseAlpha)
{
	/*�A���t�@�����t���O�������Ă����瑁�����^�[��*/
	if (this->isAddAlpha)return;

	/*�A���t�@�̌���*/
	this->alpha -= _decreaseAlpha;

	/*�O�������Ȃ��悤�ɂ���*/
	if (this->alpha < 0)
	{
		this->alpha = 0;
		this->isAddAlpha = true;
	}
}

/// <summary>
/// �t�F�[�h�C���A�E�g
/// </summary>
void Image::FadeInOut(const int _addAlpha)
{

	if (this->isAddAlpha)
	{
		this->FadeIn(_addAlpha);
	}
	else
	{
		this->FadeOut(_addAlpha);
	}
}

/// <summary>
/// �摜�̊g��
/// </summary>
bool Image::ExtendGraph(const std::vector<int> _targetPosition, const std::vector<int> _addValue)
{
	//�g�傪���e������
	int endCount = 0;

	for (int i = 0; i < position.size(); i++)
	{
		/*���W���ڕW�n�_�ɂȂ��Ă��Ȃ�������*/
		if (this->position[i] != _targetPosition[i])
		{
			//�w�肵�������ʂ����W�ɑ���
			this->position[i] += _addValue[i];
			//�����ʂ��v���X��������
			if (_addValue[i] > 0)
			{
				//�ő�l�𒴂��Ȃ��悤�ɂ���
				if (this->position[i] >= _targetPosition[i])
				{
					this->position[i] = _targetPosition[i];
					endCount++;
				}
			}
			//�����ʂ��}�C�i�X��������
			else if (_addValue[i] < 0)
			{
				//�O�������Ȃ��悤�ɂ���
				if (this->position[i] < _targetPosition[i])
				{
					this->position[i] = _targetPosition[i];
					endCount++;
				}
			}
		}
		else
		{
			endCount++;
		}
	}


	/*�I���J�E���g�����W�̃T�C�Y+1�Ɠ����������炷�ׂĂ̍��W�̈ړ����I�����Ă����*/
	if (endCount == this->position.size())
	{
		return true;
	}
	return false;
}

/// <summary>
/// �摜�̊g��
/// </summary>
void Image::ScalingGraph(const std::vector<int> _extendPosition, const std::vector<int> _reducedPosition, const std::vector<int> _scalingValue)
{ 
	int  endCount		= 0;	//�g�傪���e������
	int  scalingValue	= 0;	//�g��l
	int  targetPosition	= 0;	//�ڕW���W
	bool setFlag		= false;//endCount���ő�̎��ɃZ�b�g����t���O

	for (int i = 0; i < position.size(); i++)
	{
		/*�g�傷�邩*/
		if (this->isExtend)
		{
			scalingValue	= _scalingValue[i];
			targetPosition	= _extendPosition[i];
			setFlag			= false;
		}
		else
		{
			scalingValue	= -_scalingValue[i];
			targetPosition	= _reducedPosition[i];
			setFlag			= true;
		}
		//�w�肵�������ʂ����W�ɑ���
		this->position[i] += scalingValue;
		//�����ʂ��v���X��������
		if (scalingValue > 0)
		{
			//�ő�l�𒴂��Ȃ��悤�ɂ���
			if (this->position[i] >= targetPosition)
			{
				this->position[i] = targetPosition;
				endCount++;
			}
		}
		//�����ʂ��}�C�i�X��������
		else if (scalingValue < 0)
		{
			//�O�������Ȃ��悤�ɂ���
			if (this->position[i] < targetPosition)
			{
				this->position[i] = targetPosition;
				endCount++;
			}
		}
	}


	/*�I���J�E���g�����W�̃T�C�Y+1�Ɠ����������炷�ׂĂ̍��W�̈ړ����I�����Ă����*/
	if (endCount == this->position.size())
	{
		this->isExtend = setFlag;
	}
}