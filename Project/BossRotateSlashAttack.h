//===========================================
// @brief ��]�p���`�U��
//===========================================
#pragma once

class BossAttack;
class BossRotateSlashAttack : public BossAttack
{
public:
	 BossRotateSlashAttack(const int _attackIndex);	//�R���X�g���N�^
	~BossRotateSlashAttack();						//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override; //�`��
};

