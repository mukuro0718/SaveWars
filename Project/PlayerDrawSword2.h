//=============================================================
// @brief �v���C���[�[���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerDrawSword2 : public PlayerAction
{
public:
	 PlayerDrawSword2();	//�R���X�g���N�^
	~PlayerDrawSword2();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	short cancelableFrame;
	short frameIndex;
};

