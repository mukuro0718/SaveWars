//=============================================================
// @brief �v���C���[�񕜃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerHeal : public PlayerAction
{
public:
	 PlayerHeal();	//�R���X�g���N�^
	~PlayerHeal();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	int healValue;
	int maxHp;
};

