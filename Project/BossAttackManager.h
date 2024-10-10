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
	//�U���̎��
	enum class AttackType
	{
		NONE			= -1,
		SLASH			= 0,//�p���`
		STAB			= 1,//�˂��h���U��
		ROTATE_PUNCH	= 2,//��]�p���`
		SLAP			= 3,
		MELEE			= 4,
		KICK			= 5,
		MELEE_COMBO_3	= 6,
		SLASH_COMBO_2	= 7
	};
private:
	std::vector<BossAttack*> attack;
	int hitNumber;
};

