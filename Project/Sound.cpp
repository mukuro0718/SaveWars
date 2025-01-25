#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Sound::Sound(const int _soundHandle, const int _volume)
	: soundHandle(_soundHandle)
	, isPlay	 (false)
	, volume	 (_volume)
{
	int state = ChangeVolumeSoundMem(this->volume, this->soundHandle);
	if (state == -1)printfDx("miss");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Sound::~Sound()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// �X�V
/// </summary>
void Sound::Update()
{

}

/// <summary>
/// �Đ��t���O�𗧂Ă�
/// </summary>
void Sound::OnIsPlay()
{
	this->isPlay = true;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_BACK);
}

/// <summary>
/// �Đ��t���O������
/// </summary>
void Sound::OffIsPlay()
{
	this->isPlay = false;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
}