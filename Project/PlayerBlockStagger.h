//=============================================================
// @brief �v���C���[�K�[�h���݃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerBlockStagger : public PlayerAction
{
public:
	 PlayerBlockStagger();	//�R���X�g���N�^
	~PlayerBlockStagger();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	/*�萔*/
	const float MAX_SPEED;	//�ő�X�s�[�h
	const float DECEL;		//�����x

	/*�����o�ϐ�*/
	float nowSpeed;//���݂̑��x
	float staminaConsumption;//�X�^�~�i�����
};

