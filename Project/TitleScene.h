//==================================================
// @brief �^�C�g���V�[��
//==================================================
#pragma once

class TitleUI;
class SceneBase;
class TitleScene : public SceneBase
{
public:
	 TitleScene(); //�R���X�g���N�^
	~TitleScene();//�f�X�g���N�^

	void		Initialize	() override;		//������
	void		Finalize	() override;		//�ŏI����
	void		Update		() override;		//�X�V
	const void	Draw		() const override;//�`��

private:
	/*�ÓI�萔*/
	static constexpr int TARGET_PRESS_BUTTON_COUNT = 2;

	/*���������֐�*/
	bool IsEnd();

	/*�����o�ϐ�*/
	bool isPrevPressAnyButton;//�����{�^���������ꂽ��
	int pressButtonCount;
};

