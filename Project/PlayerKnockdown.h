//=============================================================
// @brief �v���C���[���݃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerKnockdown : public PlayerAction
{
public:
	 PlayerKnockdown();	//�R���X�g���N�^
	~PlayerKnockdown();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	float maxSpeed;
};

