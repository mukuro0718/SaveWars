//=============================================================
// @brief �v���C���[���݃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerKnockup : public PlayerAction
{
public:
	 PlayerKnockup();	//�R���X�g���N�^
	~PlayerKnockup();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
};

