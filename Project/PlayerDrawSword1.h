//=============================================================
// @brief �v���C���[�����A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerDrawSword1 : public PlayerAction
{
public:
	 PlayerDrawSword1();	//�R���X�g���N�^
	~PlayerDrawSword1();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	int frameCount;
};

