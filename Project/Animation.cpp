#include <DxLib.h>
#include "UseSTL.h"
#include "Animation.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Animation::Animation()
	: animationAttachIndex(0)
	, prevAnimation(0)
	, nowAnimation(0)
	, animationRate(0.0f)
	, prevAnimationAttachIndex(0)
	, animationTotalTime(0.0f)
	, animationPlayTime(0.0f)
	, isChange(false)
{
	this->addAnimationRate = 0.05f;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Animation::~Animation()
{
	this->animationHandle.clear();
	this->animationIndex.clear();
}

/// <summary>
/// �A�j���[�V�����̒ǉ�
/// </summary>
void Animation::Add(const int _animationHandle,const int _animatinoIndex)
{
	this->animationHandle.emplace_back(_animationHandle);
	this->animationIndex.emplace_back(_animatinoIndex);
}

/// <summary>
/// �A�j���[�V�����̃A�^�b�`
/// </summary>
void Animation::Attach(int* modelHandle)
{
	/*�A�j���[�V�����̃f�^�b�`*/
	MV1DetachAnim(*modelHandle, this->prevAnimationAttachIndex);
	MV1DetachAnim(*modelHandle, this->animationAttachIndex);

	/*�A�j���[�V�����̃A�^�b�`*/
	this->prevAnimationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->prevAnimation], this->animationHandle[this->prevAnimation], TRUE);
	this->animationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->nowAnimation], this->animationHandle[this->nowAnimation], TRUE);
	
	/*�A�j���[�V�����̑��Đ����Ԃ�ݒ�*/
	this->animationTotalTime = MV1GetAttachAnimTotalTime(*modelHandle, this->animationAttachIndex);
	
	/*�A�j���[�V�����̃u�����h����������*/
	this->animationRate = 0.0f;

	/*�A�j���[�V�����Đ����Ԃ�ݒ�*/
	MV1SetAttachAnimTime(*modelHandle, this->animationAttachIndex, this->animationPlayTime);
}
/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Animation::Play(int* _modelHandle, VECTOR& _position, const int _nextAnimation, const float _animationPlayTime)
{
	/*�������܂ŃA�^�b�`���Ă����A�j���[�V�����Ǝ��̃A�j���[�V�������Ⴄ�Ȃ�*/
	if (this->nowAnimation != _nextAnimation)
	{
		VECTOR MovePosition = MV1GetFramePosition(*_modelHandle, 1);
		float yOffset = MovePosition.y - _position.y;
		_position = MovePosition;
		_position.y -= yOffset;

		this->prevAnimation = this->nowAnimation;
		this->nowAnimation = _nextAnimation;
		this->isChange = false;
		this->animationPlayTime = 0.0f;
		MV1SetAttachAnimTime(*_modelHandle, this->animationAttachIndex, this->animationPlayTime);
		//�A�j���[�V�����̃A�^�b�`
		Attach(_modelHandle);
	}

	/*�Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�*/
	if (this->animationPlayTime >= this->animationTotalTime)
	{
		VECTOR MovePosition = MV1GetFramePosition(*_modelHandle, 1);
		float yOffset = MovePosition.y - _position.y;
		_position = MovePosition;
		_position.y -= yOffset;
		this->animationPlayTime = 0.0f;
		MV1SetAttachAnimTime(*_modelHandle, this->animationAttachIndex, this->animationPlayTime);
	}

	/*�A�j���[�V�����ύX���t���O�������Ă����牺���i�{���Ȃ炷�łɍ~��Ă���͂������A�����A�j���[�V�������Đ����悤�Ƃ���Ɨ������܂܂ɂȂ�j*/
	if (this->isChange)
	{
		this->isChange = false;
	}

	/*�A�j���[�V�����̃u�����h�����Z�b�g*/
	MV1SetAttachAnimBlendRate(*_modelHandle, this->prevAnimationAttachIndex, 1.0f - this->animationRate);
	MV1SetAttachAnimBlendRate(*_modelHandle, this->animationAttachIndex, this->animationRate);

	/*�u�����h����1�ȏゾ������*/
	if (this->animationRate >= 1.0f)
	{
		//�O�̃A�^�b�`���Ă����A�j���[�V�������f�^�b�`����
		MV1DetachAnim(*_modelHandle, this->prevAnimationAttachIndex);
	}
	else
	{
		//�u�����h���𑝉�
		this->animationRate += this->addAnimationRate;
	}

	/*�A�j���[�V�����Đ����Ԃ�i�߂�*/
	this->animationPlayTime += _animationPlayTime;
	MV1SetAttachAnimTime(*_modelHandle, this->animationAttachIndex, this->animationPlayTime);


	/*�Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������ύX�t���O�𗧂Ă�*/
	if (this->animationPlayTime >= this->animationTotalTime)
	{
		this->isChange = true;
	}
}
