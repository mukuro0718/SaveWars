//===========================================
// @brief �X���b�V���U��
//===========================================
#pragma once

class BossAttack;
class BossSlashAttack : public BossAttack
{
public:
	BossSlashAttack(const int _attackIndex);//�R���X�g���N�^
	~BossSlashAttack();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
protected:
};
