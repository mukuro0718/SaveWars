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
private:
	std::vector<BossAttack*> attack;
	int hitNumber;
};

