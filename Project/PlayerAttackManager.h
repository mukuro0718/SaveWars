#pragma once

class PlayerAttack;
class PlayerAttackManager
{
public:
	PlayerAttackManager ();//�R���X�g���N�^
	~PlayerAttackManager();//�f�X�g���N�^

	void		Initialize();				 //������
	void		Finalize  ();				 //�㏈��
	void		Update	  ();				 //�X�V
	const void	Draw	  ()const;			 //�`��
	void		OnIsStart ();//�J�n�t���O�𗧂Ă�
private:
	enum class AttackType
	{
		NONE = -1,
		MAIN_1 = 0,//�r��
		MAIN_2 = 1,//�r��
		SPECIAL = 2,//�R���{
	};
	PlayerAttack* attack;
	int hitNumber;
};

