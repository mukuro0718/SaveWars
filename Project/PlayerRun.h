//=============================================================
// @brief �v���C���[�_�b�V���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerRun : public PlayerAction
{
public:
	 PlayerRun();	//�R���X�g���N�^
	~PlayerRun();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
};

