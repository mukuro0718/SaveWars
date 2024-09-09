#include "SceneState.h"

//SceneState* Singleton<SceneState>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneState::SceneState()
	: currentScene(this->TITLE)
	, prevScene(this->TITLE)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneState::~SceneState()
{

}


void SceneState::Update()
{
	this->prevScene = this->currentScene;
}

/// <summary>
/// �V�[���̕ύX
/// </summary>
void SceneState::ChangeTitleToTutorial()
{
	this->currentScene = this->TUTORIAL;
}
void SceneState::ChangeTutorialToGame()
{
	this->currentScene = this->GAME;
}
void SceneState::ChangeGameToGameclear()
{
	this->currentScene = this->GAMECLEAR;
}
void SceneState::ChangeGameToGameover()
{
	this->currentScene = this->GAMEOVER;
}
void SceneState::ChangeResultToTitle()
{
	this->currentScene = this->TITLE;
}

/// <summary>
/// �ǂ̃V�[����
/// </summary>
const bool SceneState::IsTheCurrentStateOfTheTitle() const
{
	if (this->currentScene == this->TITLE)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheTutorial() const
{
	if (this->currentScene == this->TUTORIAL)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheGame() const
{
	if (this->currentScene == this->GAME)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheResult() const
{
	if (this->currentScene == this->MASK_RESULT)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheGameClear() const
{
	if (this->currentScene == this->GAMECLEAR)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheGameOver() const
{
	if (this->currentScene == this->GAMEOVER)return true;
	return false;
}

/// <summary>
/// ���݂ƑO�̃V�[�����قȂ��Ă��邩
/// </summary>
const bool SceneState::IsDifferentState() const
{
	if (this->currentScene != this->prevScene)return true;
	return false;
}