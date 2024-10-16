//========================================
// @brief �}�b�v�}�l�[�W���[�N���X
// �}�b�v�̊Ǘ����s��
//========================================
#pragma once
#include "Singleton.h"

class HPUI;
class ButtonUI;
class SceneUI;
class BossNameUI;
class Audience;
class UIManager : public Singleton<UIManager>
{
public:
	friend class Singleton<UIManager>;
	virtual ~UIManager();//�f�X�g���N�^

	void Initialize();		//������
	void Update();		//�X�V
	const void Draw()const;//�`��
	const bool IsContinue()const;
	const bool IsDraw()const;

private:
	/*���������֐�*/
	UIManager();//�R���X�g���N�^

	/*�����o�ϐ�*/
	std::vector<SceneUI*>	scene;
	HPUI*					hp;
	ButtonUI*				button;
	BossNameUI*				bossName;
	Audience*				audience;
};

