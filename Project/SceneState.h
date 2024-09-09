//===============================================
// @brief �V�[���`�F���W���[�N���X
// �V�[���^�C�v�̕ύX�֐�������
// �V�[���̃^�C�v��ێ�
// �V���O���g���Ŏ���
//===============================================
#pragma once
#include "Singleton.h"

class SceneState :public Singleton<SceneState>
{
public:
	friend class Singleton<SceneState>;

	void Update();

	/*�V�[���̕ύX*/
	void ChangeTitleToTutorial();
	void ChangeTutorialToGame();
	void ChangeGameToGameover();
	void ChangeGameToGameclear();
	void ChangeResultToTitle();

	/*�V�[���̔���*/
	const bool IsTheCurrentStateOfTheTitle	  () const;
	const bool IsTheCurrentStateOfTheTutorial () const;
	const bool IsTheCurrentStateOfTheGame	  () const;
	const bool IsTheCurrentStateOfTheResult   () const;
	const bool IsTheCurrentStateOfTheGameOver () const;
	const bool IsTheCurrentStateOfTheGameClear() const;

	/*���݂ƑO�̃V�[�����قȂ��Ă��邩*/
	const bool IsDifferentState()const;
private:
	/*�ÓI�萔*/
	static constexpr unsigned int TITLE		  = (1 << 0);
	static constexpr unsigned int TUTORIAL	  = (1 << 1);
	static constexpr unsigned int GAME		  = (1 << 2);
	static constexpr unsigned int GAMEOVER	  = (1 << 3);
	static constexpr unsigned int GAMECLEAR   = (1 << 4);
	static constexpr unsigned int MASK_RESULT = GAMECLEAR | GAMEOVER;

	/*���������֐�*/
	SceneState();//�R���X�g���N�^
	virtual ~SceneState();//�f�X�g���N�^

	/*�����o�ϐ�*/
	unsigned int currentScene;		//���݂̃V�[��
	unsigned int prevScene;
};