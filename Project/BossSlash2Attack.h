//===========================================
// @brief �X���b�V���U��Ver.2
//===========================================
#pragma once

class BossAttack;
class BossSlash2Attack : public BossAttack
{
public:
	BossSlash2Attack(const int _attackIndex);//�R���X�g���N�^
	~BossSlash2Attack();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
};

