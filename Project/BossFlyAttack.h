//===========================================
// @brief �X���b�V���U��
//===========================================
#pragma once
class BossAttack;
class BossFlyAttack : public BossAttack
{
public:
	BossFlyAttack(const int _attackIndex);//�R���X�g���N�^
	~BossFlyAttack();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
protected:
};

