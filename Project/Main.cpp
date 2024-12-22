#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "ScreenSetup.h"
#include "SceneManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    /*�X�N���[���ݒ�*/
    auto& screenSetup = Singleton<ScreenSetup>::GetInstance();
    if (screenSetup.Initialize() == -1) return -1; 

    /*�Q�[���{��*/
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        auto& scene = Singleton<SceneManager>::GetInstance();
        scene.Update();
    }

    // Effekseer���I������B
    Effkseer_End();
    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}