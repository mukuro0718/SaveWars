//=============================================================
// @brief �v���C���[�R���{2�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerCombo2 : public PlayerAction
{
public:
	 PlayerCombo2();	//�R���X�g���N�^
	~PlayerCombo2();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	int frameCount;
};

