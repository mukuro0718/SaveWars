#include <DxLib.h>
#include "UseJson.h"
#include "ScreenSetup.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ScreenSetup::ScreenSetup()
{
    Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ScreenSetup::~ScreenSetup()
{

}

int ScreenSetup::Initialize()
{

    /*Json�}�l�[�W���[�̃C���X�^���X�̎擾*/
    auto& json = Singleton<JsonManager>::GetInstance();

    /*���̕ϐ��ɑ��*/
    int windowHeight = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["WINDOW_HEIGHT"];
    int windowWidth  = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["WINDOW_WIDTH"];
    int colorBit     = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["COLOR_BIT"];

    SetBackgroundColor(100, 100, 100);

    // �c�w���C�u�����̏�����
    if (DxLib_Init() == -1)
    {
        // �G���[�����������璼���ɏI��
        return -1;
    }
	/*��ʂ̍쐬*/
    SetGraphMode(windowWidth, windowHeight, colorBit);
    //�t���O�𗧂Ă邱�ƂŔ񓯊����[�h���\�ɂȂ�
    SetUseASyncLoadFlag(TRUE);
    //ScreenFlip�����s���Ă����������M����҂��Ȃ�
    SetWaitVSyncFlag(FALSE);
    //�`���𗠉�ʂɂ���
    SetDrawScreen(DX_SCREEN_BACK);
    //��ʃ��[�h���E�B���h�E���[�h�ŕ`�悷�邩
#if _DEBUG
    ChangeWindowMode(TRUE);
#else
    ChangeWindowMode(FALSE);
#endif// _DEBUG
    return 0;
}