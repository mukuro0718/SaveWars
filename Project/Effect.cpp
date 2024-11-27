#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "Transform.h"
#include "Effect.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle	(_effectResourceHandle)
	, isPlayEffect			(false)
	, transform				(nullptr)
	, frameCount			(0)
	, startFrame			(0)
	, endFrame				(0)
	, firstFrame			(0)
{
	this->transform = new Transform();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Effect::~Effect()
{
	DeleteMemberInstance(this->transform);
	this->playingEffectHandle.clear();
}

/// <summary>
/// ������
/// </summary>
void Effect::Initialize()
{
	this->playingEffectHandle.emplace_back(-1);
	this->frameCount		  = 0;
	this->isPlayEffect		  = false;
	this->startFrame		  = 0;
	this->endFrame			  = 0;
}

/// <summary>
/// �X�V
/// </summary>
void Effect::Update()
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;

	if (this->frameCount == 0)
	{
		/*�G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�*/
		SetInfoToPlayTheEffect();
	}

	/*�t���[���v��*/
	//�t���[���̑���
	this->frameCount++;
	//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
	if (this->frameCount < this->startFrame)return;

	/*�G�t�F�N�g�̍X�V*/
	//�Đ��G�t�F�N�g�̃n���h��
	if (this->frameCount == this->startFrame)
	{
		this->playingEffectHandle[0] = PlayEffekseer3DEffect(this->effectResourceHandle);
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle[0], this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[0], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
	}
	// �Đ����̃G�t�F�N�g���ړ�����
	SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[0], this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//�I���t���[���𒴂��Ă�����Đ��t���O������
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = this->firstFrame;
		StopEffekseer3DEffect(this->playingEffectHandle[0]);
		this->playingEffectHandle[0] = -1;
	}
}

/// <summary>
/// �`��
/// </summary>
const void Effect::Draw()const
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;
	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}

/// <summary>
/// ���W�̐ݒ�
/// </summary>
void Effect::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
}