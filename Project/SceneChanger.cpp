#include "SceneChanger.h"

//SceneChanger* Singleton<SceneChanger>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneChanger::SceneChanger()
	:nowScene(SceneType::GAME)
	, nextSceneType(nowScene)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneChanger::~SceneChanger()
{

}

/// <summary>
/// �V�[���̓���
/// </summary>
void SceneChanger::SynchroScene()
{
	this->nowScene = this->nextSceneType;
}

/// <summary>
/// �V�[���̕ύX
/// </summary>
void SceneChanger::ChangeScene(const SceneType _nextScene)
{
	this->nextSceneType = _nextScene;
}
