//===========================================
// @brief �X���b�V���R���{�U��Ver.2
//===========================================
#pragma once

class BossAttack;
class BossSlashCombo2Attack : public BossAttack
{
public:
	BossSlashCombo2Attack(const int _attackIndex);//�R���X�g���N�^
	~BossSlashCombo2Attack();//�f�X�g���N�^

	void Initialize() override;		//������
	void Update() override;		//�X�V
	const void Draw() const override;	//�`��
};

