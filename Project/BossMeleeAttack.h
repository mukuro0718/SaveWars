//===========================================
// @brief ��]�R��U��
//===========================================
#pragma once

class BossAttack;
class BossMeleeAttack : public BossAttack
{
public:
	BossMeleeAttack(const int _attackIndex);//�R���X�g���N�^
	~BossMeleeAttack();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
private:
	int hitCheckCount;//�����蔻��̉�
	int hitCheckInterval;//�����蔻��̃C���^�[�o��
};

