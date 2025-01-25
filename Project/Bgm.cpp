#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"
#include "Bgm.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Bgm::Bgm(const int _soundHandle, const int _volume)
	: Sound(_soundHandle, _volume)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Bgm::~Bgm()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// �X�V
/// </summary>
void Bgm::Update()
{
	int checkState = CheckSoundMem(this->soundHandle);
	if (checkState == 1) printfDx("�Đ���\n");
	else if (checkState == 0) printfDx("�Đ�����Ă��Ȃ�\n");
	else if (checkState == -1) printfDx("���s\n");
}

/// <summary>
/// �Đ��t���O�𗧂Ă�
/// </summary>
void Bgm::OnIsPlay()
{
	this->isPlay = true;
	StopSoundMem(this->soundHandle);
	SetCurrentPositionSoundMem(0, this->soundHandle);
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_LOOP);
}
