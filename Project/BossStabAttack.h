//===========================================
// @brief �W�����v�U��
//===========================================
#pragma once

class BossAttack;
class BossStabAttack : public BossAttack
{
public:
	BossStabAttack(const int _attackIndex);//�R���X�g���N�^
	~BossStabAttack();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
};

