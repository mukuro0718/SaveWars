//===========================================
// @brief ��]�p���`�U��
//===========================================
#pragma once
class BossAttack;
class BossRotatePunchAttack : public BossAttack
{
public:
	BossRotatePunchAttack(const int _attackIndex);//�R���X�g���N�^
	~BossRotatePunchAttack();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override; //�`��
protected:
};

