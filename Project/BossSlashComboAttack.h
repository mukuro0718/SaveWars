//===========================================
// @brief �X���b�V���R���{�U��
//===========================================
#pragma once

class BossAttack;
class BossSlashComboAttack : public BossAttack
{
public:
	 BossSlashComboAttack(const int _attackIndex);	//�R���X�g���N�^
	~BossSlashComboAttack();						//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
private:
	short onHitCheckFrame;
	short offHitChekcFrame;
};

