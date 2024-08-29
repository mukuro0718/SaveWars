//===========================================================================
// @brief �Q�[���V�[���N���X
// �Q�[���{�҂ɕK�v�ȁA�e�N���X�̃C���X�^���X��ێ����A
// �V�[���x�[�X�N���X�Ɋ�Â����������s��
//===========================================================================
#pragma once
#include "SceneBase.h"

class GoriLib::Physics;
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
	/*�ÓI�萔*/
	static constexpr unsigned int TITLE		= (1 << 0);//�^�C�g��
	static constexpr unsigned int TUTORIAL	= (1 << 1);//�`���[�g���A��
	static constexpr unsigned int GAME		= (1 << 2);//�Q�[��
	static constexpr unsigned int RESULT	= (1 << 3);//���U���g

	/*���������֐�*/
	void ChangeState();//�Q�[���̏�Ԃ̕ύX

	unsigned int gameState;//�Q�[���̏�ԁi�{����Scene~�ɂ������������A�ق��̖����Ɣ�邽�ߕύX�j

	class GoriLib::Physics* physics;
};

