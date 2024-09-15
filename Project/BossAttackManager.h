#pragma once

class BossAttack;
class BossAttackManager
{
public:
	BossAttackManager();//�R���X�g���N�^
	~BossAttackManager();//�f�X�g���N�^

	void		 Initialize	();					//������
	void		 Finalize	();					//�㏈��
	void		 Update		();					//�X�V
	const void	 Draw		()const;			//�`��
	void		 OnIsStart	(const int _index);	//�J�n�t���O�𗧂Ă�
	const VECTOR GetPosition(const int _index);	//���W�̎擾
	enum class AttackType
	{
		NONE		   = -1,
		SLASH		   = 0,//�X���b�V��
		FLY_ATTACK	   = 1,//��эU��
		HURRICANE_KICK = 2,//��]�R��
		JUMP_ATTACK	   = 3,//�W�����v�A�^�b�N
		ROTATE_PUNCH   = 4,//��]�p���`
	};
private:
	std::vector<BossAttack*> attack;
	int hitNumber;
};

