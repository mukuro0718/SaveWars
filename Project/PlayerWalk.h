//=============================================================
// @brief �v���C���[�����A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerWalk : public PlayerAction
{
public:
	 PlayerWalk();	//�R���X�g���N�^
	~PlayerWalk();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
};

