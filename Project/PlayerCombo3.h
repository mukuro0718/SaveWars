//=============================================================
// @brief �v���C���[�R���{3�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerCombo3 : public PlayerAction
{
public:
	 PlayerCombo3();	//�R���X�g���N�^
	~PlayerCombo3();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	bool isPlay;
};

