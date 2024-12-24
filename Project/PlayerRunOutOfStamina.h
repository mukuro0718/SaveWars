//=============================================================
// @brief �v���C���[�X�^�~�i�؂�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerRunOutOfStamina : public PlayerAction
{
public:
	 PlayerRunOutOfStamina();	//�R���X�g���N�^
	~PlayerRunOutOfStamina();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	float maxSpeed;
	float staminaConsumption;
};

