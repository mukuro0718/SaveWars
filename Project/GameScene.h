//===========================================================================
// @brief �Q�[���V�[���N���X
// �Q�[���{�҂ɕK�v�ȁA�e�N���X�̃C���X�^���X��ێ����A
// �V�[���x�[�X�N���X�Ɋ�Â����������s��
//===========================================================================
#pragma once

class SceneBase;
class GameScene : public SceneBase
{
public:
	GameScene(); //�R���X�g���N�^
	~GameScene();//�f�X�g���N�^

	void		Initialize() override;		//������
	void		Finalize  () override;		//�ŏI����
	void		Update	  () override;		//�X�V
	const void	Draw	  () const override;//�`��

private:
	bool IsEnd();
};

