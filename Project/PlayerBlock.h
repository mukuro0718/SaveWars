//=============================================================
// @brief �v���C���[�_�b�V���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerBlock : public PlayerAction
{
public:
	 PlayerBlock();	//�R���X�g���N�^
	~PlayerBlock();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
};

