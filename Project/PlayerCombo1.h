//=============================================================
// @brief �v���C���[�R���{�P�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerCombo1 : public PlayerAction
{
public:
	 PlayerCombo1();	//�R���X�g���N�^
	~PlayerCombo1();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	int frameCount;
};

