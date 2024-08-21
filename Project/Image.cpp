#include <DxLib.h>
#include "Image.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Image::Image(const int _imageHandle)
	: imageHandle(_imageHandle)
	, alpha		 (this->MAX_ALPHA)
{

}

/// <summary>
/// �ʏ�`��
/// </summary>
const void Image::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawGraph(this->normal.x, this->normal.y, this->imageHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}
/// <summary>
/// �g�債�ĕ`��
/// </summary>
const void Image::DrawExtend()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->extend.leftX, this->extend.leftY, this->extend.rightX, this->extend.rightY, this->imageHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}

/// <summary>
/// �RD��Ԃɕ`��
/// </summary>
const void Image::DrawBillboard()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawBillboard3D(this->billboard.position, this->billboard.centerX, this->billboard.centerY, this->billboard.size, this->billboard.angle, this->imageHandle,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}

