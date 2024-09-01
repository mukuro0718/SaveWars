#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "GoriLib.h"
#include "Effect.h"

Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle(_effectResourceHandle)
	, playingEffectHandle(-1)
	, frameCount(0)
	, isPlayEffect(false)
{
}

/// <summary>
/// �X�V
/// </summary>
void Effect::Update(const int _startFrame, const int _endFrame, const VECTOR _position, const VECTOR _rotation, const VECTOR _scale)
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;

	/*�t���[���v��*/
	//�t���[���̑���
	this->frameCount++;
	//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
	if (this->frameCount < _startFrame)return;

	/*�G�t�F�N�g�̍X�V*/
	//�Đ��G�t�F�N�g�̃n���h��
	if (this->frameCount == _startFrame)
	{
		this->playingEffectHandle = PlayEffekseer3DEffect(this->effectResourceHandle);
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle, _rotation.x, _rotation.y, _rotation.z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle, _scale.x, _scale.y, _scale.z);
	}
	// �Đ����̃G�t�F�N�g���ړ�����
	SetPosPlayingEffekseer3DEffect(this->playingEffectHandle, _position.x, _position.y, _position.z);
	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//�I���t���[���𒴂��Ă�����Đ��t���O������
	if (this->frameCount >= _endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = 0;
		StopEffekseer3DEffect(this->playingEffectHandle);
		this->playingEffectHandle = -1;
	}
}

/// <summary>
/// �`��
/// </summary>
const void Effect::Draw()const
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (this->playingEffectHandle == -1)return;
	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}