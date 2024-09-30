#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "Transform.h"
#include "Effect.h"

Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle(_effectResourceHandle)
	, playingEffectHandle(-1)
	, frameCount(0)
	, isPlayEffect(false)
	, startFrame(0)
	, endFrame(0)
	, transform(nullptr)
{
	this->transform = new Transform();
}

/// <summary>
/// �X�V
/// </summary>
void Effect::Update()
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;

	/*�G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�*/
	SetInfoToPlayTheEffect();

	/*�t���[���v��*/
	//�t���[���̑���
	this->frameCount++;
	//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
	if (this->frameCount < this->startFrame)return;

	/*�G�t�F�N�g�̍X�V*/
	//�Đ��G�t�F�N�g�̃n���h��
	if (this->frameCount == this->startFrame)
	{
		this->playingEffectHandle = PlayEffekseer3DEffect(this->effectResourceHandle);
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
	}
	// �Đ����̃G�t�F�N�g���ړ�����
	SetPosPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//�I���t���[���𒴂��Ă�����Đ��t���O������
	if (this->frameCount >= this->endFrame)
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

/// <summary>
/// vector<float>��VECTOR�ɕϊ�
/// </summary>
const VECTOR Effect::Convert(std::vector<float> _in)const
{
	VECTOR out = VGet(0.0f, 0.0f, 0.0f);
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];
	return out;
}
