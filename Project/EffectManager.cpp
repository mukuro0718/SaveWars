#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <vector>
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::EffectManager()
{
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}

/// <summary>
/// ������
/// </summary>
void EffectManager::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void EffectManager::Finalize()
{

}