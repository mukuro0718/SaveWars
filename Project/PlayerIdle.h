//=============================================================
// @brief �v���C���[�ҋ@�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerIdle : public PlayerAction
{
public:
	 PlayerIdle();	//�R���X�g���N�^
	~PlayerIdle();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
};

