//===========================================
// @brief �W�����v�U��
//===========================================
#pragma once
class BossAttack;
class BossJumpAttack : public BossAttack
{
public:
	 BossJumpAttack(const int _attackIndex);//�R���X�g���N�^
	~BossJumpAttack();						//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
};

