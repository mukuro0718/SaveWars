//=============================================================
// @brief �v���C���[���U���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerStrongAttack : public PlayerAction
{
public:
	 PlayerStrongAttack();	//�R���X�g���N�^
	~PlayerStrongAttack();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	bool isPlay;
	bool isCharge;
	int frameCount;
};

