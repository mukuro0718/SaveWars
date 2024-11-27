#include <DxLib.h>
#include "EffekseerForDXLib.h"
//#include "GoriLib.h"
//#include "UseSTL.h"
//#include "UseJson.h"
//#include "JsonLoader.h"
//#include "LoadingAsset.h"
#include "ScreenSetup.h"
//#include "SceneChanger.h"
#include "SceneManager.h"
//#include "Debug.h"
//#include "InputManager.h"
//#include "CameraManager.h"
//#include "MapManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    /*�V���O���g���N���X�̃C���X�^���X���쐬*/
    //Singleton<JsonLoader>   ::CreateInstance();
    //Singleton<JsonManager>  ::CreateInstance();
    //Singleton<LoadingAsset> ::CreateInstance();
    //Singleton<Debug>        ::CreateInstance();
    //Singleton<CameraManager>::CreateInstance();
    //Singleton<MapManager>   ::CreateInstance();
    //Singleton<InputManager> ::CreateInstance();
    //Singleton<ScreenSetup>  ::CreateInstance();
    //Singleton<SceneManager> ::CreateInstance();
    //Singleton<SceneChanger> ::CreateInstance();

    /*�X�N���[���ݒ�*/
    auto& screenSetup = Singleton<ScreenSetup>::GetInstance();
    if (screenSetup.Initialize() == -1) return -1; 

    /*�Q�[���{��*/
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        auto& scene = Singleton<SceneManager>::GetInstance();
        scene.Update();
    }

    /*�V���O���g���N���X�̃C���X�^���X���쐬*/
    //Singleton<SceneChanger> ::DeleteInstance();
    //Singleton<SceneManager> ::DeleteInstance();
    //Singleton<ScreenSetup>  ::DeleteInstance();
    //Singleton<InputManager> ::DeleteInstance();
    //Singleton<LoadingAsset> ::DeleteInstance();
    //Singleton<MapManager>   ::DeleteInstance();
    //Singleton<Debug>        ::DeleteInstance();
    //Singleton<CameraManager>::DeleteInstance();
    //Singleton<JsonLoader>   ::DeleteInstance();
    //Singleton<JsonManager>  ::DeleteInstance();

    // Effekseer���I������B
    Effkseer_End();
    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}