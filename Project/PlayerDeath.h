//=============================================================
// @brief �v���C���[�f�X�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerDeath : public PlayerAction
{
public:
	 PlayerDeath();	//�R���X�g���N�^
	~PlayerDeath();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
};

